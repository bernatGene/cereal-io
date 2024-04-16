using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Linq;
using System.IO.Ports; // Requires .NET 4 in Project Settings

public class Cereal
{
    private SerialPort _serial;
    private static List<byte> _broken = new List<byte>();
    private float[] _floatChannels = new float[128];
    private int[] _intChannels = new int[128];

    private void SetChannelValue(byte datatype, uint channel, byte[] data)
    {
        if (channel > 128)
        {
            return; //bad Channel
        }
        if (datatype == (byte)'I')
        {
            int value = BitConverter.ToInt32(data, 0);
            _intChannels[channel] = value;
        }
        else if (datatype == (byte)'F')
        {
            float value = BitConverter.ToSingle(data, 0);
            _floatChannels[channel] = value;
        }
    }
    private void HandleBuffer(byte[] newBuffer)
    {
        byte[] buffer = new byte[_broken.Count + newBuffer.Length];

        Array.Copy(newBuffer, 0, buffer, _broken.Count, newBuffer.Length);
        for (int i = 0; i < _broken.Count; i++)
        {
            buffer[i] = _broken[i];
        }
        for (int i = 0; i < buffer.Length; i++)
        {
            if (buffer.Length - i < 10) // not enough left to contain a datagram
            {
                _broken.Clear();
                for (int j = 0; (i + j) < buffer.Length; j++)
                {
                    _broken.Add(buffer[i + j]);
                }
                return;
            }
            if (buffer[i] != (byte)'B') // not the start of a datagram
            {
                continue;
            }
            bool B2 = buffer[i + 1] == (byte)'B';
            bool E1 = buffer[i + 8] == (byte)'E';
            bool E2 = buffer[i + 9] == (byte)'E';
            if (!(B2 && E1 && E2)) // datagram is malformed; continue
            {
                continue;
            }
            uint channel = buffer[i + 2];
            byte datatype = buffer[i + 3];
            byte[] data = new byte[4];
            Array.Copy(buffer, i + 4, data, 0, 4);
            i += 9;
            SetChannelValue(datatype, channel, data);
        }
    }

    public IEnumerator ReadCereal()
    {
        while (true)
        {
            if (_serial == null || !_serial.IsOpen)
            {
                yield return new WaitForSeconds(1 / 5.0f);
            }
            int bytesToRead = _serial.BytesToRead;
            if (bytesToRead > 0)
            {
                byte[] buff = new byte[bytesToRead];
                int read = _serial.Read(buff, 0, bytesToRead);
                if (read > 0)
                {
                    HandleBuffer(buff);
                    yield return null;
                }
            }
            else
            {
                yield return null;
            }
        }
    }

    private void setData(byte channel, byte datatype, byte[] datagram, byte[] data)
    {
        datagram[0] = (byte)'B';
        datagram[1] = channel;
        datagram[2] = datatype;
        Array.Copy(data, 0, datagram, 3, 4);
        datagram[7] = (byte)'\n';
    }

    private byte[] ToDatagram(byte channel, byte datatype, float value)
    {
        byte[] datagram = new byte[8];
        byte[] data = BitConverter.GetBytes(value);
        setData(channel, datatype, datagram, data);
        return datagram;
    }
    private byte[] ToDatagram(byte channel, byte datatype, int value)
    {
        byte[] datagram = new byte[8];
        byte[] data = BitConverter.GetBytes(value);
        setData(channel, datatype, datagram, data);
        return datagram;
    }
    private static string FormatBytes(Byte[] bytes)
    {
        string value = "";
        foreach (var byt in bytes)
            value += string.Format("{0:X2} ", byt);

        return value;
    }

    public void SendInt(byte channel, int value)
    {
        byte[] datagram = ToDatagram(channel, (byte)'I', value);
        Debug.Log(FormatBytes(datagram));
        _serial.Write(datagram, 0, 8);
    }
    public void SendFloat(byte channel, float value)
    {
        byte[] datagram = ToDatagram(channel, (byte)'F', value);
        _serial.Write(datagram, 0, 8);
    }

    public void InitCereal(string port, int baudRate = 9600)
    {
        try
        {
            _serial = new SerialPort(port, baudRate)
            {
                Encoding = System.Text.Encoding.UTF8,
                DtrEnable = true
            };

            _serial.Open();
        }
        catch (Exception e)
        {
            Debug.Log(e);
        }
    }

    public void Disconnect()
    {
        if (_serial != null)
        {
            if (_serial.IsOpen)
            {
                _serial.Close();
            }

            _serial.Dispose();
            _serial = null;

            Debug.Log("Disconnected");
        }
    }


    public float ReadFloat(uint channel)
    {
        return _floatChannels[channel];
    }
    public int ReadInt(uint channel)
    {
        return _intChannels[channel];
    }

}


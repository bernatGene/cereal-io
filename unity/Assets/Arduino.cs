using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using System;
using System.Linq;
using System.IO.Ports; // Requires .NET 4 in Project Settings

public class SerialTest : MonoBehaviour
{
    public Dropdown PortsDropdown;

    private List<string> _ports;
    public Text ConnectionText;
    private SerialPort _serial;

    void Start()
    {
        RefreshPortsDropdown();
    }

        // For simplicity we are reading data in our Update loop of our script. However, if you have a lot of data to read, I would recommend creating a separate thread and reading the data from that instead of the main UI thread.
    void Update()
    {
        // Make sure we have a serial port and that the connection is open
        if (_serial != null && _serial.IsOpen)
        {
            // Check to see if there are any bytes available to read
            int bytesToRead = _serial.BytesToRead;
            if (bytesToRead > 0)
            {
                // Debug.Log($"Bytes to read: {bytesToRead}");

                // Create our buffer to store the data
                byte[] buff = new byte[bytesToRead];

                // Read the available data into our buffer
                int read = _serial.Read(buff, 0, bytesToRead);

                // Check if we were able to read anything
                if (read > 0)
                {
                    // Debug.Log($"Read {read} bytes");
                    PrintBytes(ref buff);
                }
                else
                {
                    Debug.Log("Didn't read anything.");
                }
            }
        }
    }

    // A convenience method for printing out bytes to the Unity console
    private void PrintBytes(ref byte[] bytes)
    {
        // StringBuilder sb = new StringBuilder();
        string sb = "Read: ";
        foreach (byte b in bytes)
        {
            char c = (char)b;

            sb += $"{c}";
        }
        Debug.Log(sb);
    }


    public void RefreshPortsDropdown()
    {
        // Remove all the previous options
        PortsDropdown.ClearOptions();

        // Get port names
        string[] portNames = SerialPort.GetPortNames();
        _ports = portNames.ToList();

        // Add the port names to our options
        PortsDropdown.AddOptions(_ports);
    }

    public void ConnectToPort()
    {
        // Get the port we want to connect to from the dropdown
        string port = _ports[PortsDropdown.value];

        try
        {
            // Attempt to create our serial port using 9600 as our baud rate which matches the baud rate we set in the Arduino Sketch we created earlier.
            _serial = new SerialPort(port, 9600)
            {
                Encoding = System.Text.Encoding.UTF8,
                DtrEnable = true
            };

            // Open up our serial connection
            _serial.Open();

            ConnectionText.text = $"Connected to {port}";
            Debug.Log(ConnectionText.text);
        }
        catch (Exception e)
        {
            ConnectionText.text = e.Message;
        }
    }


    public void Disconnect()
    {
        if (_serial != null)
        {
            // Close the connection if it is open
            if (_serial.IsOpen)
            {
                _serial.Close();
            }

            // Release any resources being used
            _serial.Dispose();
            _serial = null;

            if (ConnectionText != null)
            {
                ConnectionText.text = "";
            }
            Debug.Log("Disconnected");
        }
    }

    // Make sure to close our connection if the script is destroyed
    void OnDestroy()
    {
        Disconnect();
    }



}

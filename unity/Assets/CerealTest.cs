using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using System;
using System.Linq;
using System.IO.Ports; // Requires .NET 4 in Project Settings


public class CerealTest : MonoBehaviour
{
    public Dropdown PortsDropdown;

    public bool ReadSerial = false;
    public bool WriteSerial = false;
    public bool WriteSerialFloat = false;

    private List<string> _ports;
    public Text ConnectionText;
    private Cereal cereal;
    private float lastWrite;
    public int lastValueSent = 0;
    public float lastValueSentFloat = 0.0f;

    void Start()
    {
        RefreshPortsDropdown();
        cereal = new Cereal();
        lastWrite = Time.time;
    }

    void Update()
    {
        if (ReadSerial)
        {
            int count = cereal.ReadInt(0);
            float x = cereal.ReadFloat(1);
            float y = cereal.ReadFloat(2);
            float z = cereal.ReadFloat(3);
            Debug.Log($"Read count({count}) x: {x}, y: {y}, z: {z}");
        }
        if (WriteSerial && lastWrite + 0.1 < Time.time)
        {
            lastWrite = Time.time;
            cereal.SendInt(1, lastValueSent);
            Debug.Log($"Sending int value {lastValueSent}");
        }
        else if (WriteSerialFloat && lastWrite + 0.1 < Time.time)
        {
            lastWrite = Time.time;
            cereal.SendFloat(2, lastValueSentFloat);
            Debug.Log($"Sending int value {lastValueSentFloat}");
        }
    }

    public void RefreshPortsDropdown()
    {
        PortsDropdown.ClearOptions();

        string[] portNames = SerialPort.GetPortNames();
        _ports = portNames.ToList();

        PortsDropdown.AddOptions(_ports);
    }

    public void ConnectToPort()
    {
        string port = _ports[PortsDropdown.value];
        cereal.InitCereal(port, 9600);
        StartCoroutine(cereal.ReadCereal());
    }

    void OnDestroy()
    {
        cereal.Disconnect();
    }
}

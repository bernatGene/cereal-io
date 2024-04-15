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
    public bool ReDebug = false;
    public bool WriteSerial = false;

    private List<string> _ports;
    public Text ConnectionText;
    private Cereal cereal;
    private float lastWrite;
    private int lastValueSent = 0;

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
        if (ReDebug)
        {
            int debug = cereal.ReadInt(99);
            Debug.Log($"Redebug {debug}");
        }
        if (WriteSerial && lastWrite + 1.0 < Time.time)
        {
            lastWrite = Time.time;
            lastValueSent += 1;
            lastValueSent %= 5;
            cereal.SendInt(1, lastValueSent);
            Debug.Log($"Sending int value {lastValueSent}");
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
        cereal.InitCereal(port, 512);
        StartCoroutine(cereal.ReadCereal());
    }

    void OnDestroy()
    {
        cereal.Disconnect();
    }
}

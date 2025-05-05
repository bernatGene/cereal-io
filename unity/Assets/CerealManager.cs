using System;
using System.IO.Ports;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

//This code displays a list of available serial ports in the console and allows you to connect to a selected port.
public class CerealManager : MonoBehaviour
{
    Cereal cereal;
    public TMP_Dropdown portDropdown; // Dropdown UI element to display available ports.

    string[] ports; // Array to hold the names of available serial ports.
    void Start()
    {
        cereal = new Cereal(); // Create an instance of the Cereal class.
        portDropdown.onValueChanged.AddListener(delegate { ConnectToPort(portDropdown.options[portDropdown.value].text); }); // Add a listener to the dropdown to call ConnectToPort when a port is selected.
        ports = SerialPort.GetPortNames(); // Get the names of available serial ports.
        //ListAvailableSerialPorts();
        portDropdown.ClearOptions(); // Clear any existing options in the dropdown.
        portDropdown.AddOptions(new List<string>(ports)); // Add the available ports to the dropdown.
    }
    void ListAvailableSerialPorts()
    {
        // Get a list of serial port names.
        ports = SerialPort.GetPortNames();

        Debug.Log("The following serial ports were found:");

        // Display each port name to the console.
        foreach (string port in ports)
        {
            Debug.Log(port);
        }
    }

    public void ConnectToPort(string port)
    {
        cereal.InitCereal(port, 9600);
        StartCoroutine(cereal.ReadCereal());
        Debug.Log("Connected to port: " + port); // Log the selected port to the console.
    }

    public void SendInt(byte channel, int value)
    {
        cereal.SendInt(channel, value); // Send an integer value to the specified channel.
    }

    public void SendFloat(byte channel, float value)
    {
        cereal.SendFloat(channel, value); // Send a float value to the specified channel.
    }

    public int ReadInt(byte channel)
    {
        int value = cereal.ReadInt(channel); // Read an integer value from the specified channel.
        Debug.Log("Read Int: " + value); // Log the read value to the console.
        return value;
    }
    public float ReadFloat(byte channel)
    {
        float value = cereal.ReadFloat(channel); // Read a float value from the specified channel.
        Debug.Log("Read Float: " + value); // Log the read value to the console.
        return value;
    }
}

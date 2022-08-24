using System;
using Obi;
using UnityEngine;
 
public class LogAnywhere : MonoBehaviour
{
    public string filename = "";
    void OnEnable() { Application.logMessageReceived += Log;  }
    void OnDisable() { Application.logMessageReceived -= Log; }
    string d = System.Environment.GetFolderPath(
        System.Environment.SpecialFolder.Desktop) + "/YOUR_LOGS/";

    public void Start()
    {
        var filepath = "";
        if (filename == "")
        {
            filepath = d + "rope.txt";
        }
        else
        {
            filepath = d + filename +".txt";
        }

        System.IO.File.CreateText(filepath);
        var propery = gameObject.GetComponent<CRProperty>();
        string info = String.Format("Young’s modulus E={0},shear modulus G={1},Density={2},R={3},PDIteration={4}",
            propery.E, propery.G, propery.Density, propery.R, propery.PdIteration) + "\n";
        Debug.Log(info);
        
    }

    public void Log(string logString, string stackTrace, LogType type)
    {
        var filepath = "";
        if (filename == "")
        {
            filepath = d + "rope.txt";
        }
        else
        {
            filepath = d + filename +".txt";
        }
 
        try {
            System.IO.File.AppendAllText(filepath, logString + "\n");
        }
        catch { }
    }
}
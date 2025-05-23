# cereal-io
Serial Communication between Arduino and Unity

## *documentation*

This library allows for a "shared-memory-like" communication between Arduino and unity using Serial communication. 

The main idea is that we have 128 float and 128 channels that communicate values between the two 
environments. Example:

```c++
// Arduino code
#include "Cereal.h"

Cereal cereal(9600);

void setup()
{
  cereal.begin();
}

float x, y, z = 0;
long count = 0;

void loop()
{
  count += 1;
  count %= 10;
  cereal.sendInt(0, count);

  x += .1;
  y += .002;
  z += .003;

  cereal.sendFloat(0, x); // float and int channels don't overlap
  cereal.sendFloat(1, y);
  cereal.sendFloat(2, z);
  cereal.passiveListen(100); // Use this instead of `delay()` to avoid blocking.
}
```

This can be read in unity like so:

```c#
using System;
using System.Linq;
using System.IO.Ports; // Requires .NET 4 in Project Settings

public class YourScriptName : Monobehavior {

  private Cereal cereal;

  void Start()
  {
    cereal = new Cereal();
    ConnectToPort();
  }

  void Update()
  {
    // will contain values from arduino
    int count = cereal.ReadInt(0);
    float x = cereal.ReadFloat(0);
    float y = cereal.ReadFloat(1);
    float z = cereal.ReadFloat(2);
  }

  public void ConnectToPort() // this must be called at void Start
  {
    string port = "Write your PORT name here";
    cereal.InitCereal(port, 9600);
    StartCoroutine(cereal.ReadCereal());
  }
}
```
It also works in the opposite direction

```c#
//unity
cereal.SendInt(1, 42); // send and read channels also don't overlap
cereal.SendFloat(1, 23.2f); // floats not doubles
```

```c++
//arduino
cereal.passiveListen(60); //stop writing for 60ms and only read 
int value = cereal.readInt(1); // 42
float othervalue = cereal.readFloat(1); // 23.2
```
It is important that you include `cereal.passiveListen(60)` or `cereal.readCereal()` in your arduino code before perfoming any read of values being sent to it by Unity. These functions are necessary for the bi-directional Serial communication to work as intended.

## Setting things up


Setting the arduino environment should be as easy as copying the `Cereal` library into the libs directory
of you arduino project. (path in this repo: `/arudino/lib/Cereal`).

Setting up the unity environment requires a tiny bit more work. Roughly:

- Setting up .NET4 in project.
  - Go to `Edit > Project Settings > Player > Other Settings > Configuration > Api Compatibility Level` and set it to `.NET Framework`.
  ![image](https://github.com/bernatGene/cereal-io/assets/39168013/af866ff5-48b1-41ff-ae45-f2c0ca78fb67)
- Copying `Cereal.cs` into your assets. 
- Creating a Game Object to contain your unity inputs.
- Connecting game objects to functions that send or read the values you want.

Alternatively you can drag and drop the `Cereal_IO_V3.unitypackage` found in this repository under the `unity` folder, on your assets folder and use the given template scene.

A full-feature working example can be found in:

- Arduino code: `/arduino/src/SendAndReadCereal.ino`
- Unity code: `/unity/Assets/CeralTest.cs`

## Serial Communication Port Selection

We need to be able to tell Unity what Port the computer is using for communication with the Arduino. 
While we can manually edit the port name on our `Cereal.cs` script, a much better way is to incorporate some sort of UI element that let's us pick the correct port.
For this we have introduced a new wrapper code called `CerealManager.cs` and a full implementation of it, a prefab containing all the necessary UI elements can be found as a `package` on: `unity/CerealManagerPackage.unitypackage`.

This wrapper contains it's own definitions for the `SendInt()`, `SendFloat()`, `ReadInt()` and `ReadFloat()` functions.
It is highly recommended that you use this implementation of the Cereal code for your Unity builds, as it will allow you to pick the correct Port for Serial Communication through a simple UI element.

*Disclaimer*: This library is currently not thoroughly tested. It has only been tested for
the UNO R3 board.

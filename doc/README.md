## How to use the programm:

### Edit the Settingsfile.json

#### The settings.json file contains the settings used for rendering. These include: 

##### The screen:

The screen

```bashrc
    "screen": {
        "dpi": 1000,
         "dimensions": [1.92, 1.08],
         "position": [0, 0, 0],   
         "rotation": [0, 0, 0],  // rotation of the camera in euler angles
         "depth": 1
     }

```
consists of the dpi (dots per inch), that corresponds to the resolution of the rendered image.  
Dimensions describes the dimensions of the screen in the scene, consisting of x and y.  
Here the combination of dpi : 1000 and dimension : [1.92, 1.08] correnponds to an image with resolution [1920,1080].  
   
The screen also has a position which refers to the position of the vanishing point or observer. The screen also has a resolution that is described using euler angles. [0,0,0] means the camera is pointing straight up. One can rotate the camera with for example [90,0,0], which would correspond to a camera pointing in the direction of y axis.  
The depth is used for the distance between the observer and the screen.

##### The medium:

```bashrc
    "medium": {
        "ambient": [0.1, 0.1, 0.1],
        "index": 1,
        "recursion": 1
    }

```

The medium describes the empty scene. It consists of the ambient lighting, the refraction index of the "air" and the number of bounces.

##### The lights:

```bashrc
    "sources": [
        {
            "position": [0, 5, 3],
            "intensity": [1, 1, 1]
        }
    ]

```

The lights are pointlights. They only have a position and not a scale or rotation. They also have an intensity.

##### The objects:

```bashrc
    "objects": [
        {"sphere": {
            "position": [-1, 0, 3],
            "radius": 0.5,
            "color": {
                "ambient": [1.0, 0.05, 0.05],
                "diffuse": [1.0, 0.05, 0.05],
                "specular": [1.0, 1, 1],
                "reflected": [1.0, 0.05, 0.05],
                "refracted": [1.0, 0.05, 0.05],
                "shininess": 1
                },
            "index": 1
            }
        }
    ]

```

There are different objects. These include infinite planes, spheres and cubes. 
Each object has a location, rotation(sometimes irrelevant) and scale(sometimes irrelevant). They also have a color, consisting of different components, aswell as a refraction index.

#### An [example](../settings.json) can be found in the main directory

#### Additional information:

```bashrc
    "screen": {
        "dpi": 128,
         "dimensions": [4, 2],
         "position": [0, 0, 0],   
         "rotation": [0, 0, 0],  // rotation of the camera in euler angles
         "depth": 1
     }

```

is equivalent to:

```bashrc
    "screen": {
        "dpi": 128,
         "dimensions": [4, 2],
         "position": [0, 0, 1],   
         "observer": [0,0,0]
     }

```

We opted for the version above for ease of use, since this version does not require any manual calculation to achieve a desired image.





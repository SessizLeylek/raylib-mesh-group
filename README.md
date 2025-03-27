# Raylib Mesh Group

## Overview

Raylib Mesh Group (RMG) is a lightweight file format and utility set for handling multiple Raylib meshes in a compact, binary format. It provides functions to export and import tightly packed meshes for efficient storage and retrieval, minimizing memory overhead and reducing file size.

## Features

- Designed to be used with [raylib](https://github.com/raysan5/raylib).
- Simple and fast implementation in C.
- Serialize and deserialize multiple Raylib Mesh objects into a single binary file.
- Easy to embed multiple meshes by importing with a byte buffer.

## Additional Notes

- Doesn't support exporting/importing animation data.
- Don't have much C experience, so it may have bugs and downsides.
- Feel free to submit pull requests or fork to improve functionality or optimize performance!

## Usage

```
#include "raylib.h"
#define RAYLIB_MESH_GROUP_H
#include "raylib_mg.h"
```

### Exporting Mesh Group

```
// Generate a sample array
Mesh meshArray[4] = {
    GenMeshCube(1, 1, 1),
    GenMeshCone(1, 1, 10),
    GenMeshCylinder(1, 1, 10),
    GenMeshSphere(1, 10, 10)
};

// Export Mesh Array
rmgExport(4, meshArray, "output.rmg");
```

### Importing Mesh Group

#### Importing from an embedded buffer

```
int meshCount;
int bufferSize = ...
unsigned char* buffer = ...
Mesh* meshes = rmgDecode(buffer, bufferSize, &meshCount);
```

#### Importing from file

```
int meshCount;
int bufferSize;
unsigned char* buffer = LoadFileData("output.rmg", &bufferSize);
Mesh* meshes = rmgDecode(buffer, bufferSize, &meshCount);
```

### Creating Models and Freeing the Memory

```
// Load Models
Model mdlCube = LoadModelFromMesh(meshes[0]);
Model mdlCone = LoadModelFromMesh(meshes[1]);
Model mdlCyln = LoadModelFromMesh(meshes[2]);
Model mdlSphr = LoadModelFromMesh(meshes[3]);

// Unload Mesh Group
rmgUnload(meshes, meshCount);
```
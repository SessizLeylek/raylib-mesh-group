#include "raylib.h"
#define RAYLIB_MESH_GROUP_H
#include "raylib_mg.h"

int main()
{
    InitWindow(900, 900, "Example");

    ////////////////////////////////////////////////////////
    //                     EXPORTING                      //
    ////////////////////////////////////////////////////////

    // Generate a sample array
    Mesh meshArray[4] = {
        GenMeshCube(1, 1, 1),
        GenMeshCone(1, 1, 10),
        GenMeshCylinder(1, 1, 10),
        GenMeshSphere(1, 10, 10)
    };

    // Export Mesh Array
    rmgExport(4, meshArray, "output.rmg");

    // Unload Meshes
    UnloadMesh(meshArray[0]);
    UnloadMesh(meshArray[1]);
    UnloadMesh(meshArray[2]);
    UnloadMesh(meshArray[3]);

    ////////////////////////////////////////////////////////
    //                     IMPORTING                      //
    ////////////////////////////////////////////////////////

    // Import Mesh Array
    int meshCount = 0;
    int fileSize = 0;
    unsigned char* file = LoadFileData("output.rmg", &fileSize);
    Mesh* meshes = rmgDecode(file, fileSize, &meshCount);
    
    // Load Models
    Model mdlCube = LoadModelFromMesh(meshes[0]);
    Model mdlCone = LoadModelFromMesh(meshes[1]);
    Model mdlCyln = LoadModelFromMesh(meshes[2]);
    Model mdlSphr = LoadModelFromMesh(meshes[3]);

    // Unload Mesh Group
    rmgUnload(meshes, meshCount);

    ////////////////////////////////////////////////////////
    //                     RENDERING                      //
    ////////////////////////////////////////////////////////

    // Render Cycle
    Camera cam = {{-10, 10, 0}, {0, 0, 0}, {0, 1, 0}, 90, CAMERA_PERSPECTIVE};
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RED);
        BeginMode3D(cam);

        DrawModel(mdlCube, (Vector3){3, 0, 0}, 1.0f, YELLOW);
        DrawModel(mdlCone, (Vector3){1, 0, 0}, 1.0f, GREEN);
        DrawModel(mdlCyln, (Vector3){-1, 0, 0}, 1.0f, LIGHTGRAY);
        DrawModel(mdlSphr, (Vector3){-3, 0, 0}, 1.0f, BLUE);

        EndMode3D();
        EndDrawing();
    }

    return 0;
}
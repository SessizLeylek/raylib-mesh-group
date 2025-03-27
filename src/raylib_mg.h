#ifdef RAYLIB_MESH_GROUP_H
#ifdef RAYLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exports mesh to a rmg file
void rmgExport(int meshCount, Mesh* meshArray, const char* fileName)
{
    FILE* file = fopen(fileName, "wb");  // Open in binary write mode
    if (!file) {
        printf("Failed to open file for writing: %s\n", fileName);
        return;
    }

    fwrite(&meshCount, sizeof(int), 1, file);  // Write mesh count

    for (int i = 0; i < meshCount; i++)
    {
        Mesh* mesh = &meshArray[i];

        // Write mesh metadata
        fwrite(&mesh->vertexCount, sizeof(int), 1, file);
        fwrite(&mesh->triangleCount, sizeof(int), 1, file);

        // Mark, if fields are available
        if(mesh->texcoords) fwrite("1", sizeof(char), 1, file);
        else fwrite("0", sizeof(char), 1, file);
        if(mesh->texcoords2) fwrite("1", sizeof(char), 1, file);
        else fwrite("0", sizeof(char), 1, file);
        if(mesh->normals) fwrite("1", sizeof(char), 1, file);
        else fwrite("0", sizeof(char), 1, file);
        if(mesh->tangents) fwrite("1", sizeof(char), 1, file);
        else fwrite("0", sizeof(char), 1, file);
        if(mesh->colors) fwrite("1", sizeof(char), 1, file);
        else fwrite("0", sizeof(char), 1, file);
        if(mesh->indices) fwrite("1", sizeof(char), 1, file);
        else fwrite("0", sizeof(char), 1, file);

        // Write vertex positions (3 floats per vertex)
        fwrite(mesh->vertices, sizeof(float), mesh->vertexCount * 3, file);

        // Write texture coordinates if available (2 floats per vertex)
        if (mesh->texcoords) {
            fwrite(mesh->texcoords, sizeof(float), mesh->vertexCount * 2, file);
        }
        
        // Write 2nd texture coordinates if available (2 floats per vertex)
        if (mesh->texcoords2) {
            fwrite(mesh->texcoords2, sizeof(float), mesh->vertexCount * 2, file);
        }

        // Write normals if available (3 floats per vertex)
        if (mesh->normals) {
            fwrite(mesh->normals, sizeof(float), mesh->vertexCount * 3, file);
        }

        // Write tangents if available (4 floats per vertex)
        if (mesh->tangents) {
            fwrite(mesh->tangents, sizeof(float), mesh->vertexCount * 4, file);
        }

        // Write colors if available (4 floats per vertex)
        if (mesh->colors) {
            fwrite(mesh->colors, sizeof(float), mesh->vertexCount * 4, file);
        }

        // Write indices if available (3 * triangleCount, stored as unsigned shorts)
        if (mesh->indices) {
            fwrite(mesh->indices, sizeof(unsigned short), mesh->triangleCount * 3, file);
        }

        // TODO Write animation data
    }

    fclose(file);
}

// Decodes rmg file into a mesh array
// Use LoadFileData() to import rmg file
// Don't forget to free after getting the meshes
Mesh* rmgDecode(unsigned char* rmgData, int dataSize, int* out_meshCount)
{
    const unsigned char* ptr = rmgData;  // Pointer to traverse the buffer

    // Read mesh count
    memcpy(out_meshCount, ptr, sizeof(int));
    ptr += sizeof(int);

    Mesh* meshArray = (Mesh*)malloc(sizeof(Mesh) * (*out_meshCount));
    if (!meshArray) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    for (int i = 0; i < *out_meshCount; i++)
    {
        Mesh* mesh = &meshArray[i];

        // Read mesh metadata
        memcpy(&mesh->vertexCount, ptr, sizeof(int));
        ptr += sizeof(int);
        memcpy(&mesh->triangleCount, ptr, sizeof(int));
        ptr += sizeof(int);

        // Read field markers
        char hasTexcoords, hasTexcoords2, hasNormals, hasTangents, hasColors, hasIndices;
        hasTexcoords = *ptr++;
        hasTexcoords2 = *ptr++;
        hasNormals = *ptr++;
        hasTangents = *ptr++;
        hasColors = *ptr++;
        hasIndices = *ptr++;

        // Read vertex positions (3 floats per vertex)
        mesh->vertices = (float*)malloc(sizeof(float) * mesh->vertexCount * 3);
        memcpy(mesh->vertices, ptr, sizeof(float) * mesh->vertexCount * 3);
        ptr += sizeof(float) * mesh->vertexCount * 3;

        // Read texture coordinates if available (2 floats per vertex)
        if (hasTexcoords == '1') {
            mesh->texcoords = (float*)malloc(sizeof(float) * mesh->vertexCount * 2);
            memcpy(mesh->texcoords, ptr, sizeof(float) * mesh->vertexCount * 2);
            ptr += sizeof(float) * mesh->vertexCount * 2;
        } else {
            mesh->texcoords = NULL;
        }

        // Read second texture coordinates if available (2 floats per vertex)
        if (hasTexcoords2 == '1') {
            mesh->texcoords2 = (float*)malloc(sizeof(float) * mesh->vertexCount * 2);
            memcpy(mesh->texcoords2, ptr, sizeof(float) * mesh->vertexCount * 2);
            ptr += sizeof(float) * mesh->vertexCount * 2;
        } else {
            mesh->texcoords2 = NULL;
        }

        // Read normals if available (3 floats per vertex)
        if (hasNormals == '1') {
            mesh->normals = (float*)malloc(sizeof(float) * mesh->vertexCount * 3);
            memcpy(mesh->normals, ptr, sizeof(float) * mesh->vertexCount * 3);
            ptr += sizeof(float) * mesh->vertexCount * 3;
        } else {
            mesh->normals = NULL;
        }

        // Read tangents if available (4 floats per vertex)
        if (hasTangents == '1') {
            mesh->tangents = (float*)malloc(sizeof(float) * mesh->vertexCount * 4);
            memcpy(mesh->tangents, ptr, sizeof(float) * mesh->vertexCount * 4);
            ptr += sizeof(float) * mesh->vertexCount * 4;
        } else {
            mesh->tangents = NULL;
        }

        // Read colors if available (4 floats per vertex)
        if (hasColors == '1') {
            mesh->colors = (unsigned char*)malloc(sizeof(float) * mesh->vertexCount * 4);
            memcpy(mesh->colors, ptr, sizeof(float) * mesh->vertexCount * 4);
            ptr += sizeof(float) * mesh->vertexCount * 4;
        } else {
            mesh->colors = NULL;
        }

        // Read indices if available (3 * triangleCount, stored as unsigned shorts)
        if (hasIndices == '1') {
            mesh->indices = (unsigned short*)malloc(sizeof(unsigned short) * mesh->triangleCount * 3);
            memcpy(mesh->indices, ptr, sizeof(unsigned short) * mesh->triangleCount * 3);
            ptr += sizeof(unsigned short) * mesh->triangleCount * 3;
        } else {
            mesh->indices = NULL;
        }

        // TODO Read animation data
        mesh->animVertices = NULL;
        mesh->animNormals = NULL;
        mesh->boneIds = NULL;
        mesh->boneWeights = NULL;
        mesh->boneMatrices = NULL;
        mesh->boneCount = 0;

        UploadMesh(mesh, false);
    }

    return meshArray;
}

// Frees mesh group from memory
void rmgUnload(Mesh* meshGroup, int meshCount)
{
    for(int i = 0; i < meshCount; i++)
    {
        // Free mesh data
        free(meshGroup[i].vertices);
        free(meshGroup[i].texcoords);
        free(meshGroup[i].texcoords2);
        free(meshGroup[i].normals);
        free(meshGroup[i].tangents);
        free(meshGroup[i].colors);
        free(meshGroup[i].indices);

        // TODO Free animation data
    }

    free(meshGroup);
}

#endif
#endif
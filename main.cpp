#include <fbxsdk.h>
#include "Common.h"

bool ProcessMesh(FbxMesh* pMesh)
{
    int uvSetCount = pMesh->GetElementUVCount();
    FBXSDK_printf(" (Mesh UV count:%d)\n", uvSetCount);
    if (uvSetCount > 1) {
        for (int i = uvSetCount - 1; i > 0; --i)
        {
            pMesh->RemoveElementUV(pMesh->GetElementUV(i));
        }
        return true;
    }
    return false;
}

bool RemoveUVChannelsRecursive(FbxNode* node, int depth)
{
    bool modified = false;
    FBXSDK_printf("%*s%s", depth+1, "-", node->GetName());
    int num = node->GetNodeAttributeCount();
    if (num == 0) {
        FBXSDK_printf("\n");
    }
    for (int index=0; index<num; index++) {
        FbxNodeAttribute *nodeAttribute = node->GetNodeAttributeByIndex(index);
        FbxNodeAttribute::EType lAttributeType = nodeAttribute->GetAttributeType();
        bool result;
        switch (lAttributeType) {
            case FbxNodeAttribute::eMesh:
                result = ProcessMesh((FbxMesh *) nodeAttribute);
                modified = modified || result;
                break;
            default:
                FBXSDK_printf("\n");
                break;
        }
    }

    ++depth;
    int count = node->GetChildCount();
    for (int i=0; i<count; i++)
    {
        FbxNode* child = node->GetChild(i);
        bool result = RemoveUVChannelsRecursive(child, depth);
        modified = modified || result;
    }
    return modified;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        FBXSDK_printf("Usage: ./fbxcleanup <fbx_file>\n");
        return 1;
    }

    const char* pFileName = argv[1];

    //FBX SDK Default Manager
    FbxManager* lSdkManager = NULL;

    //Scene to load from file
    FbxScene* lScene = NULL;

    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);

    // Load the scene.
    bool lResult = LoadScene(lSdkManager, lScene, pFileName);

    if(lResult == false)
    {
        FBXSDK_printf("\n\nAn error occurred while loading the scene...");
    }
    else {
        //Get the first node in the scene
        FbxNode* rootNode = lScene->GetRootNode();
        bool modified = RemoveUVChannelsRecursive(rootNode, 0);
        if (modified)
        {
            SaveScene(lSdkManager, lScene, pFileName);
        }
    }

    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(lSdkManager, lResult);
    return 0;
}
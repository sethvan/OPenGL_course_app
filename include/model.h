/*Class for containing groups of meshes and textures*/

#ifndef _INCLUDED_MODEL_H
#define _INCLUDED_MODEL_H

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <memory>
#include <string>
#include <vector>

#include "mesh.h"
#include "texture.h"

class Model {
  private:
   std::vector<std::unique_ptr<Mesh>> meshList;
   std::vector<std::unique_ptr<Texture>> textureList;
   std::vector<unsigned int> meshToTex;

   void loadNode( aiNode* node, const aiScene* scene );  // scene holds all data and node holds data for one
   void loadMesh( aiMesh* mesh, const aiScene* scene );  // needs to find the scene to reference the correct textures
   void loadMaterials( const aiScene* scene );           // materials are all things involved with textures/lighting

  public:
   Model();

   void loadModel( const std::string& filename );
   void renderModel();
   void clearModel();

   ~Model();
};

#endif  // _INCLUDED_MODEL_H
#include "model.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <algorithm>
#include <assimp/Importer.hpp>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "mesh.h"
#include "texture.h"

static std::string trim_underscore_num( std::string& filename ) {
   auto posUnderscore = filename.find_last_of( "_" );
   if ( posUnderscore == std::string::npos ) {
      return filename;
   }
   auto posDot = filename.find_last_of( "." );
   size_t length;
   if ( ( length = posDot - posUnderscore ) < 0 ) {
      return filename;
   }
   auto startPos = posUnderscore++;
   while ( posUnderscore != posDot && std::isdigit( filename[ posUnderscore++ ] ) )
      ;
   if ( posUnderscore != posDot ) {
      return filename;
   }
   return filename.replace( startPos, length, "" );
}

Model::Model() {
}

// same as course
void Model::loadModel( const std::string& filename ) {
   Assimp::Importer importer;
   const aiScene* scene =
       importer.ReadFile( filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals |
                                        aiProcess_JoinIdenticalVertices );
   if ( !scene ) {
      std::cerr << "Model " << filename << " failed to load\n";
      throw std::runtime_error( importer.GetErrorString() );
   }

   loadNode( scene->mRootNode, scene );
   loadMaterials( scene );
}

// same as course
void Model::loadNode( aiNode* node, const aiScene* scene ) {
   for ( size_t i = 0; i < node->mNumMeshes; i++ ) {
      loadMesh( scene->mMeshes[ node->mMeshes[ i ] ], scene );
   }

   for ( size_t i = 0; i < node->mNumChildren; i++ ) {
      loadNode( node->mChildren[ i ], scene );
   }
}

// 2 lines different
void Model::loadMesh( aiMesh* mesh, const aiScene* scene ) {
   std::vector<GLfloat> vertices;
   std::vector<unsigned int> indices;

   for ( size_t i = 0; i < mesh->mNumVertices; i++ ) {
      vertices.insert( vertices.end(), { mesh->mVertices[ i ].x, mesh->mVertices[ i ].y, mesh->mVertices[ i ].z } );

      if ( mesh->mTextureCoords[ 0 ] ) {
         vertices.insert( vertices.end(), { mesh->mTextureCoords[ 0 ][ i ].x, mesh->mTextureCoords[ 0 ][ i ].y } );
      } else {
         vertices.insert( vertices.end(), { 0.0f, 0.0f } );
      }
      // have to make normals negative here because shaders have normals as positive when more sommon to d0o the
      // negative this is due to instructor preference/laziness
      vertices.insert( vertices.end(), { -mesh->mNormals[ i ].x, -mesh->mNormals[ i ].y, -mesh->mNormals[ i ].z } );
   }

   for ( size_t i = 0; i < mesh->mNumFaces; i++ ) {
      aiFace face = mesh->mFaces[ i ];
      for ( size_t j = 0; j < face.mNumIndices; j++ ) {
         indices.push_back( face.mIndices[ j ] );
      }
   }

   meshList.push_back( std::make_unique<Mesh>() );  // different
   // Mesh newMesh;
   // newMesh.createMesh( vertices.data(), indices.data(), vertices.size(), indices.size() );
   // meshList.push_back( std::move( newMesh ) );

   meshList.back()->createMesh( vertices.data(), indices.data(), vertices.size(), indices.size() );  // different

   meshToTex.push_back( mesh->mMaterialIndex );  // to match up texture index to mesh index between arrays
}

void Model::loadMaterials( const aiScene* scene ) {
   for ( size_t i = 0; i < scene->mNumMaterials; ++i ) {
      textureList.push_back( nullptr );
   }
   for ( size_t i = 0; i < scene->mNumMaterials; ++i ) {
      aiMaterial* material = scene->mMaterials[ i ];

      if ( material->GetTextureCount( aiTextureType_DIFFUSE ) ) {
         aiString path;
         if ( material->GetTexture( aiTextureType_DIFFUSE, 0, &path ) == AI_SUCCESS ) {
            int idx = std::string( path.data ).find_last_of( "\\/" );
            std::string file_name = std::string( path.data ).substr( idx + 1 );
            file_name = trim_underscore_num( file_name );

            std::string texPath = std::string( "res/textures/" ) + file_name;

            auto texture = std::make_unique<Texture>( texPath );

            if ( !texture->loadTexture() ) {
               std::cerr << "Failed to load texture at: " << texPath << '\n';
               textureList[ i ] = std::move( std::make_unique<Texture>( "res/textures/plain.png" ) );
               textureList[ i ]->loadTextureA();
            } else {
               textureList[ i ] = std::move( texture );
            }
         }
      } else {
         textureList[ i ] = std::move( std::make_unique<Texture>( "res/textures/plain.png" ) );
         textureList[ i ]->loadTextureA();
      }
   }
}

void Model::renderModel() {
   for ( size_t i = 0; i < meshList.size(); i++ ) {
      unsigned int materialIndex = meshToTex[ i ];

      if ( materialIndex < textureList.size() ) {
         textureList[ materialIndex ]->useTexture();
      } else {
         printf( "Index not in range!\n" );
      }
      meshList[ i ]->renderMesh();
   }
}

void Model::clearModel() {
   meshList.clear();
   textureList.clear();
}

Model::~Model() {
}
#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader_manager.h"

struct vertex
{
	glm::vec3 position;
	glm::vec3 normal;    
    glm::vec3 tangent;
};

struct texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class mesh
{
public:
	std::vector<vertex> vertices;
	std::vector<int> indices;
    GLuint VAO, VBO, EBO;

	mesh(std::vector<vertex> vertices, std::vector<int> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
		setup_mesh();
	}

	void display_mesh(GLuint shader_id)
	{
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (GLuint)indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
	}

private:
    void setup_mesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, tangent));
        glBindVertexArray(0);
    }
};

class model
{
public:
    std::vector<mesh> meshes;
    std::string dir;

    model() = default;

    model(std::string const& path)
    {
        load_model(path);
    }

    void display_model(GLuint shader_id)
    {
        for (int i = 0; i < meshes.size(); i++)
            meshes[i].display_mesh(shader_id);
    }

private:
    void load_model(std::string const &path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "Error occurred when loading the model:" << importer.GetErrorString() << std::endl;
            return;
        }
        dir = path.substr(0, path.find_last_of('/'));
        check_node(scene->mRootNode, scene);
    }

    void check_node(aiNode* node, const aiScene* scene)
    {
        for (int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(check_mesh(mesh, scene));
        }
        for (int i = 0; i < node->mNumChildren; i++)
            check_node(node->mChildren[i], scene);
    }

    mesh check_mesh(aiMesh* param_mesh, const aiScene* scene)
    {
        std::vector<vertex> vertices;
        std::vector<int> indices;

        for (int i = 0; i < param_mesh->mNumVertices; i++)
        {
            vertex vertex;
            glm::vec3 vector;

            vector.x = param_mesh->mVertices[i].x;
            vector.y = param_mesh->mVertices[i].y;
            vector.z = param_mesh->mVertices[i].z;
            vertex.position = vector;
            if (param_mesh->HasNormals())
            {
                vector.x = param_mesh->mNormals[i].x;
                vector.y = param_mesh->mNormals[i].y;
                vector.z = param_mesh->mNormals[i].z;
                vertex.normal = vector;
            }
            if (param_mesh->mTextureCoords[0])
            {
                vector.x = param_mesh->mTangents[i].x;
                vector.y = param_mesh->mTangents[i].y;
                vector.z = param_mesh->mTangents[i].z;
                vertex.tangent = vector;
            }
            vertices.push_back(vertex);
        }
        for (int i = 0; i < param_mesh->mNumFaces; i++)
        {
            aiFace face = param_mesh->mFaces[i];
            for (int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
   
        return mesh(vertices, indices);
    }
};
#endif

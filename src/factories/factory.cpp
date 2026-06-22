#include "factory.h"
#include "../stb_image.h"
#include <algorithm>
#include <cfloat>

Factory::Factory(
    std::unordered_map<unsigned int, PhysicsComponent>& physicsComponents,
    std::unordered_map<unsigned int, RenderComponent>& renderComponents,
    std::unordered_map<unsigned int, TransformComponent>& transformComponents):
physicsComponents(physicsComponents),
renderComponents(renderComponents),
transformComponents(transformComponents) {
}

Factory::~Factory() {
    glDeleteBuffers(VBOs.size(), VBOs.data());
    glDeleteVertexArrays(VAOs.size(), VAOs.data());
    glDeleteTextures(textures.size(), textures.data());
}

unsigned int Factory::make_camera(glm::vec3 position, glm::vec3 eulers) {

    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;

    transformComponents[entities_made] = transform;

    PhysicsComponent physics;
    physics.velocity = {0.0f, 0.0f, 0.0f};
    physics.eulerVelocity = {0.0f, 0.0f, 0.0f};
    physics.radius = 0.25f;
    physicsComponents[entities_made] = physics;

    return entities_made++;
}

void Factory::make_cube(glm::vec3 position, glm::vec3 eulers, 
    glm::vec3 eulerVelocity) {

	TransformComponent transform;
	transform.position = position;
	transform.eulers = eulers;
	transformComponents[entities_made] = transform;

	PhysicsComponent physics;
	physics.velocity = {0.0f, 0.0f, 0.0f};
	physics.eulerVelocity = eulerVelocity;
	physics.radius = glm::length(glm::vec3(0.25f, 0.25f, 0.25f));
	physicsComponents[entities_made] = physics;
	
	RenderComponent render = make_cube_mesh({0.25f, 0.25f, 0.25f});
	render.material = make_texture("img/kucing_mewing.jpg");
	renderComponents[entities_made++] = render;
}

float Factory::compute_obj_radius(const char* filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return 0.0f;
    }

    std::string line;
    bool foundVertex = false;
    glm::vec3 minBounds(FLT_MAX);
    glm::vec3 maxBounds(-FLT_MAX);

    while (std::getline(file, line)) {
        std::vector<std::string> words = split(line, " ");
        if (words.empty() || words[0] != "v" || words.size() < 4) {
            continue;
        }

        glm::vec3 position(
            std::stof(words[1]),
            std::stof(words[2]),
            std::stof(words[3]));

        minBounds = glm::min(minBounds, position);
        maxBounds = glm::max(maxBounds, position);
        foundVertex = true;
    }

    if (!foundVertex) {
        return 0.0f;
    }

    glm::vec3 extents = maxBounds - minBounds;
    return std::max(extents.x, std::max(extents.y, extents.z)) * 0.5f;
}

void Factory::make_furina(glm::vec3 position, glm::vec3 eulers) {

	TransformComponent transform;
	transform.position = position;
	transform.eulers = eulers;
	transformComponents[entities_made] = transform;

	PhysicsComponent physics;
	physics.velocity = {0.0f, 0.0f, 0.0f};
	physics.eulerVelocity = {0.0f, 0.0f, 0.0f};
	physics.radius = compute_obj_radius("models/furina/furina.obj");
	if (physics.radius <= 0.0f) {
		physics.radius = 1.0f;
	}
	physicsComponents[entities_made] = physics;
	
    glm::mat4 preTransform = glm::mat4(1.0f);
	preTransform = glm::rotate(preTransform, 
        glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
    preTransform = glm::rotate(preTransform, 
        glm::radians(90.0f), { 0.0f, 1.0f, 0.0f });
	RenderComponent render = make_obj_mesh(
        "models/furina/furina.obj", preTransform);
	renderComponents[entities_made++] = render;
}

void Factory::make_grassfield(glm::vec3 position) {

	TransformComponent transform;
	transform.position = position;
	transformComponents[entities_made] = transform;
	
    glm::mat4 preTransform = glm::mat4(1.0f);
	preTransform = glm::rotate(preTransform, 
        glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
    preTransform = glm::rotate(preTransform, 
        glm::radians(90.0f), { 0.0f, 1.0f, 0.0f });
	RenderComponent render = make_obj_mesh(
        "models/grassfield/grassfield.obj", preTransform);
	renderComponents[entities_made++] = render;
}

RenderComponent Factory::make_cube_mesh(glm::vec3 size) {

    float l = size.x;
    float w = size.y;
    float h = size.z;

    std::vector<float> vertices = {
         l,  w, -h, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
         l, -w, -h, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
        -l, -w, -h, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
        -l, -w, -h, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
        -l,  w, -h, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
         l,  w, -h, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,

        -l, -w,  h, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         l, -w,  h, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         l,  w,  h, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
         l,  w,  h, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -l,  w,  h, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -l, -w,  h, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

        -l,  w,  h, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -l,  w, -h, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -l, -w, -h, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -l, -w, -h, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -l, -w,  h, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -l,  w,  h, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,

         l, -w, -h, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         l,  w, -h, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         l,  w,  h, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         l,  w,  h, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         l, -w,  h, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         l, -w, -h, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,

        -l, -w, -h, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
         l, -w, -h, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
         l, -w,  h, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         l, -w,  h, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
        -l, -w,  h, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
        -l, -w, -h, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,

         l,  w,  h, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         l,  w, -h, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -l,  w, -h, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -l,  w, -h, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -l,  w,  h, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         l,  w,  h, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    VAOs.push_back(VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    VBOs.push_back(VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), 
        vertices.data(), GL_STATIC_DRAW);
    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
    glEnableVertexAttribArray(0);
    //texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void*)12);
    glEnableVertexAttribArray(1);
    //normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 32, (void*)20);
    glEnableVertexAttribArray(2);

    RenderComponent record;
    record.VAO = VAO;
    record.vertexCount = 36;
    return record;
}

RenderComponent Factory::make_obj_mesh(
    const char* filepath, glm::mat4 preTransform) {

    std::vector<glm::vec3> v;
    std::vector<glm::vec2> vt;
    std::vector<glm::vec3> vn;
    std::unordered_map<std::string, std::vector<float>> materialVertices;
    std::unordered_map<std::string, unsigned int> materialTextures;
    std::vector<std::string> materialOrder;
    std::unordered_map<std::string, size_t> materialIndices;

    std::string line;
    std::vector<std::string> words;
    std::string currentMaterial = "__default__";

    std::ifstream file;
    file.open(filepath);
    while (std::getline(file, line)) {

        words = split(line, " ");
        if (words.empty()) {
            continue;
        }

        if (!words[0].compare("v")) {
            v.push_back(read_vec3(words, preTransform, 1.0f));
        }

        else if (!words[0].compare("vt")) {
            vt.push_back(read_vec2(words));
        }

        else if (!words[0].compare("vn")) {
            vn.push_back(read_vec3(words, preTransform, 0.0f));
        }

        else if (!words[0].compare("usemtl") && words.size() > 1) {
            currentMaterial = words[1];
            if (materialIndices.find(currentMaterial) == materialIndices.end()) {
                materialIndices[currentMaterial] = materialOrder.size();
                materialOrder.push_back(currentMaterial);
            }
        }

        else if (!words[0].compare("f")) {
            if (materialVertices.find(currentMaterial) == materialVertices.end()) {
                materialVertices[currentMaterial] = std::vector<float>();
            }
            read_face(words, v, vt, vn, materialVertices[currentMaterial]);
        }
    }
    file.close();

    std::string objPath = filepath;
    std::string mtlPath = objPath;
    size_t lastDot = objPath.find_last_of('.');
    size_t lastSlash = objPath.find_last_of("/\\");
    if (lastDot != std::string::npos && (lastSlash == std::string::npos || lastDot > lastSlash)) {
        mtlPath.replace(lastDot, objPath.size() - lastDot, ".mtl");
    } else {
        mtlPath += ".mtl";
    }

    if (std::ifstream(mtlPath).good()) {
        materialTextures = load_material_textures(mtlPath.c_str());
    }

    std::vector<unsigned int> submeshMaterials;
    std::vector<unsigned int> submeshVAOs;
    std::vector<unsigned int> submeshVertexCounts;

    for (const std::string& materialName : materialOrder) {
        std::vector<float>& vertices = materialVertices[materialName];
        if (vertices.empty()) {
            continue;
        }

        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        VAOs.push_back(VAO);
        glBindVertexArray(VAO);

        unsigned int VBO;
        glGenBuffers(1, &VBO);
        VBOs.push_back(VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), 
            vertices.data(), GL_STATIC_DRAW);
        //position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
        glEnableVertexAttribArray(0);
        //texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void*)12);
        glEnableVertexAttribArray(1);
        //normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 32, (void*)20);
        glEnableVertexAttribArray(2);

        unsigned int materialTexture = 0;
        if (materialTextures.find(materialName) != materialTextures.end()) {
            materialTexture = materialTextures[materialName];
        } else if (!materialTextures.empty()) {
            materialTexture = materialTextures.begin()->second;
        } else {
            materialTexture = make_texture("img/paper.jpg");
        }

        submeshMaterials.push_back(materialTexture);
        submeshVAOs.push_back(VAO);
        submeshVertexCounts.push_back(vertices.size() / 8);
    }

    RenderComponent render;
    render.VAO = submeshVAOs.empty() ? 0 : submeshVAOs[0];
    render.material = submeshMaterials.empty() ? 0 : submeshMaterials[0];
    render.vertexCount = submeshVertexCounts.empty() ? 0 : submeshVertexCounts[0];
    render.VAOs = submeshVAOs;
    render.materials = submeshMaterials;
    render.vertexCounts = submeshVertexCounts;
    return render;
}

glm::vec3 Factory::read_vec3(std::vector<std::string> words,
    glm::mat4 preTransform, float w) {
    return glm::vec3(
        preTransform 
        * glm::vec4(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]), w)
    );
}

glm::vec2 Factory::read_vec2(std::vector<std::string> words) {
    return glm::vec2(std::stof(words[1]), std::stof(words[2]));
}

void Factory::read_face(std::vector<std::string> words, 
    std::vector<glm::vec3>& v, std::vector<glm::vec2>& vt, 
    std::vector<glm::vec3>& vn, std::vector<float>& vertices) {
    
    size_t triangleCount = words.size() - 3;

    for (size_t i = 0; i < triangleCount; ++i) {
        read_corner(words[1], v, vt, vn, vertices);
        read_corner(words[2 + i], v, vt, vn, vertices);
        read_corner(words[3 + i], v, vt, vn, vertices);
    }

}

void Factory::read_corner(std::string description, 
    std::vector<glm::vec3>& v, std::vector<glm::vec2>& vt, 
    std::vector<glm::vec3>& vn, std::vector<float>& vertices) {
    
    std::vector<std::string> v_vt_vn = split(description, "/");

    //position
    glm::vec3 pos = v[std::stol(v_vt_vn[0]) - 1];
    vertices.push_back(pos[0]);
    vertices.push_back(pos[1]);
    vertices.push_back(pos[2]);

    //tex coord
    glm::vec2 texcoord = vt[std::stol(v_vt_vn[1]) - 1];
    vertices.push_back(texcoord[0]);
    vertices.push_back(texcoord[1]);

    //normal
    glm::vec3 normal = vn[std::stol(v_vt_vn[2]) - 1];
    vertices.push_back(normal[0]);
    vertices.push_back(normal[1]);
    vertices.push_back(normal[2]);

}

unsigned int Factory::make_texture(const char* filename) {

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(
        filename, &width, &height, &channels, STBI_rgb_alpha);

    if (data == nullptr) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

	//make the texture
    unsigned int texture;
	glGenTextures(1, &texture);
    textures.push_back(texture);
    glBindTexture(GL_TEXTURE_2D, texture);
	
    //load data
    glTexImage2D(GL_TEXTURE_2D, 
        0, GL_RGBA, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data);

    //free data
	stbi_image_free(data);

    //Configure sampler
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

std::unordered_map<std::string, unsigned int> Factory::load_material_textures(const char* mtl_filepath) {
    std::unordered_map<std::string, unsigned int> materialTextures;
    std::string mtlPath = mtl_filepath;
    std::string mtlDir = "";
    size_t lastSlash = mtlPath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        mtlDir = mtlPath.substr(0, lastSlash + 1);
    }

    std::ifstream file(mtlPath);
    if (!file.is_open()) {
        return materialTextures;
    }

    std::string currentMaterial;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> words = split(line, " ");
        if (words.empty()) {
            continue;
        }

        if (!words[0].compare("newmtl") && words.size() > 1) {
            currentMaterial = words[1];
        }

        else if (!words[0].compare("map_Kd") && words.size() > 1 && !currentMaterial.empty()) {
            std::string texturePath = words[1];
            if (texturePath.find("://") == std::string::npos &&
                (texturePath.empty() || texturePath[0] != '/' && (texturePath.size() < 2 || texturePath[1] != ':'))) {
                texturePath = mtlDir + texturePath;
            }

            materialTextures[currentMaterial] = make_texture(texturePath.c_str());
        }
    }

    return materialTextures;
}

unsigned int Factory::make_texture_from_mtl(const char* mtl_filepath, const char* default_texture) {
    std::unordered_map<std::string, unsigned int> materialTextures = load_material_textures(mtl_filepath);
    if (materialTextures.empty()) {
        return make_texture(default_texture);
    }

    return materialTextures.begin()->second;
}
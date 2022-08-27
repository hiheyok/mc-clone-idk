#ifndef CHUNK_R_H_
#define CHUNK_R_H_

#include "Mesh/Mesher.h"

#include "../Camera/camera.h"
#include "../frustum/frustum.h"
#include "../OpenGL/shader/shader.h"

#include "../../Utils/MathHelper.h"
#include "../OpenGL/Texture/stb_image.h"
#include <unordered_map>
#include <deque>
#include <queue>

struct DrawArraysIndirectCommand {
	unsigned int  count;
	unsigned int  instanceCount;
	unsigned int  first;
	unsigned int  baseInstance;
};

struct ChunkRenderDataBufferAddress {
	int offset = 0;
	size_t size = 0;
	int x = 0;
	int y = 0;
	int z = 0;
};


class ChunkRenderer {

public:
	ChunkRenderer() {


	}

	void init(GLFWwindow* window_, Camera* camera_) {

		window = window_;
		camera = camera_;
		SolidShader = new Shader;
		SolidShader->init("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);
		glGenBuffers(1, &SSBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, GPUBufferSizeSolid, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, IBO);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, GPUBufferSizeTransparent / 500, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, (void*)(0 * sizeof(unsigned int)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, (void*)(1 * sizeof(unsigned int)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, GPUSSBOMAXSIZE, nullptr, GL_DYNAMIC_COPY);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void DeleteChunk(long long int ChunkID) {
		for (int i = 0; i < ChunkRenderListSolid.size(); i++) {
			if (getChunkID(ChunkRenderListSolid[i].x, ChunkRenderListSolid[i].y, ChunkRenderListSolid[i].z) == ChunkID) {
				MeshList[ChunkID] = false;
				MeshList.erase(ChunkID);
				ChunkRenderListSolid.erase(ChunkRenderListSolid.begin() + i);
				ChunkRenderListSolidOffsetLookup.erase(ChunkID);
				break;
			}
		}
	}

	void AddChunk(ChunkMesh* mesh) {

		long long int ChunkID = getChunkID(mesh->pos);

		/*if (chunk.isEmpty()) {
			return;
		}*/


		size_t MeshSizeSolid = mesh->vertices.size() * sizeof(unsigned int);
		size_t MeshSizeTransparent = mesh->transparentVertices.size() * sizeof(unsigned int);

		if (mesh->vertices.size() == 0) {
			MeshList[getChunkID(mesh->pos)] = true;
			//std::cout << "ERROR: No data " << mesh->pos.x << ", " << mesh->pos.y << ", " << mesh->pos.z << "\n";
	//		delete mesh;
			return;
		}

		ChunkRenderDataBufferAddress renderdata;
		renderdata.x = mesh->pos.x;
		renderdata.y = mesh->pos.y;
		renderdata.z = mesh->pos.z;

		if (ChunkRenderListSolid.size() == 0) {
			renderdata.offset = 0;
			renderdata.size = MeshSizeSolid;
			insertData(VBO, GL_ARRAY_BUFFER, renderdata.offset, &mesh->vertices);
			ChunkRenderListSolid.insert(ChunkRenderListSolid.begin(), renderdata);
			MeshList[getChunkID(mesh->pos)] = true;
			ChunkRenderListSolidOffsetLookup[getChunkID(mesh->pos)] = 0;
			//	std::cout << "[ Info ]: Added Chunk: " << mesh->cpos.x << ", " << mesh->cpos.y << ", " << mesh->cpos.z << "\n";
		}
		else {
			if (ChunkRenderListSolid.back().offset + ChunkRenderListSolid.back().size + MeshSizeSolid < GPUBufferSizeSolid) {
				renderdata.offset = ChunkRenderListSolid.back().offset + ChunkRenderListSolid.back().size;
				renderdata.size = MeshSizeSolid;
				insertData(VBO, GL_ARRAY_BUFFER, renderdata.offset, &mesh->vertices);
				ChunkRenderListSolid.emplace_back(renderdata);
				MeshList[getChunkID(mesh->pos)] = true;
				ChunkRenderListSolidOffsetLookup[getChunkID(mesh->pos)] = ChunkRenderListSolid.size() - 1;
			}
			else {
				bool added = false;
				for (int i = 0; i < ChunkRenderListSolid.size(); i++) {
					if (ChunkRenderListSolid[i].offset + ChunkRenderListSolid[i].size + MeshSizeSolid < ChunkRenderListSolid[i + 1].offset) {
						renderdata.offset = ChunkRenderListSolid[i].offset + ChunkRenderListSolid[i].size;
						renderdata.size = MeshSizeSolid;
						insertData(VBO, GL_ARRAY_BUFFER, renderdata.offset, &mesh->vertices);
						ChunkRenderListSolid.insert(ChunkRenderListSolid.begin() + i + 1, renderdata);
						MeshList[getChunkID(mesh->pos)] = true;
						ChunkRenderListSolidOffsetLookup[getChunkID(mesh->pos)] = i;
						added = true;
						//std::cout << "[ Info ]: Added Chunk: " << mesh->cpos.x << ", " << mesh->cpos.y << ", " << mesh->cpos.z << "\n";
						break;
					}
				}
				if (added == false) {
					std::cout << "ERROR (GPU): Out of memory\n";
				}
			}
		}
	//	delete mesh;
	}



	void genSpreadCache(int rd, int rh) {
		SpreadCacheRenderDistance = rd;
		SpreadCacheRenderHeight = rh;

		SpreadCache.clear();

		const int SBSize = rd * 2 + 1;
		const int SBSize2x = SBSize * SBSize;

		spread.resize(SBSize * SBSize * SBSize);

		FIBO.emplace(FIBO.end(), ((rd)*SBSize2x) + ((rd)*SBSize) + (rd));
		spread[((rd)*SBSize2x) + ((rd)*SBSize) + (rd)] = rd;
		SpreadCache.push_back(glm::vec3(rd, rd, rd));

		while (!FIBO.empty()) {
			int index = FIBO.front();

			int lz = index % SBSize;
			int lx = index / (SBSize2x);
			int ly = (index % (SBSize2x)) / SBSize;

			int spreadlvl = spread[(lx * SBSize2x) + (ly * SBSize) + lz];

			FIBO.pop_front();

			if (spread[((lx + 1) * SBSize2x) + (ly * SBSize) + lz] + 1 < spreadlvl) {
				if (abs(ly) - rd < rh) {
					spread[((lx + 1) * SBSize2x) + (ly * SBSize) + lz] = spreadlvl - 1;
					SpreadCache.push_back(glm::vec3(lx, ly, lz));
					FIBO.emplace(FIBO.end(), ((lx + 1) * SBSize2x) + (ly * SBSize) + lz);
				}
			}
			if (spread[((lx - 1) * SBSize2x) + (ly * SBSize) + lz] + 1 < spreadlvl) {
				if (abs(ly) - rd < rh) {
					spread[((lx - 1) * SBSize2x) + (ly * SBSize) + lz] = spreadlvl - 1;
					SpreadCache.push_back(glm::vec3(lx, ly, lz));
					FIBO.emplace(FIBO.end(), ((lx - 1) * SBSize2x) + (ly * SBSize) + lz);
				}
			}
			if (spread[(lx * SBSize2x) + ((ly + 1) * SBSize) + lz] + 1 < spreadlvl) {
				if (abs(ly) - rd < rh) {
					spread[(lx * SBSize2x) + ((ly + 1) * SBSize) + lz] = spreadlvl - 1;
					SpreadCache.push_back(glm::vec3(lx, ly, lz));
					FIBO.emplace(FIBO.end(), (lx * SBSize2x) + ((ly + 1) * SBSize) + lz);
				}
			}
			if (spread[(lx * SBSize2x) + ((ly - 1) * SBSize) + lz] + 1 < spreadlvl) {
				if (abs(ly) - rd < rh) {
					spread[(lx * SBSize2x) + ((ly - 1) * SBSize) + lz] = spreadlvl - 1;
					SpreadCache.push_back(glm::vec3(lx, ly, lz));
					FIBO.emplace(FIBO.end(), (lx * SBSize2x) + ((ly - 1) * SBSize) + lz);
				}
			}
			if (spread[(lx * SBSize2x) + (ly * SBSize) + (lz + 1)] + 1 < spreadlvl) {
				if (abs(ly) - rd < rh) {
					spread[(lx * SBSize2x) + (ly * SBSize) + (lz + 1)] = spreadlvl - 1;
					SpreadCache.push_back(glm::vec3(lx, ly, lz));
					FIBO.emplace(FIBO.end(), (lx * SBSize2x) + (ly * SBSize) + (lz + 1));
				}
			}
			if (spread[(lx * SBSize2x) + (ly * SBSize) + (lz - 1)] + 1 < spreadlvl) {
				if (abs(ly) - rd < rh) {
					spread[(lx * SBSize2x) + (ly * SBSize) + (lz - 1)] = spreadlvl - 1;
					SpreadCache.push_back(glm::vec3(lx, ly, lz));
					FIBO.emplace(FIBO.end(), (lx * SBSize2x) + (ly * SBSize) + (lz - 1));
				}
			}
		}

		spread.empty();
		spread.resize(1);
	}

	void GenCallDrawCommands() {

		DrawArraysIndirectCommandListSolid.clear();

		glm::ivec3 Pos = glm::ivec3(floor(PlayerPosition.x / 16), floor(PlayerPosition.y / 16), floor(PlayerPosition.z / 16));
		
		/*if (SpreadCacheRenderDistance != rd || SpreadCacheRenderHeight != rh) {
			genSpreadCache(rd,rh);
		}
		
		for (int i = 0; i < SpreadCache.size(); i++) {

			int inverseI = SpreadCache.size() - (i + 1);

			glm::vec3 SpreadPos = SpreadCache[inverseI];

			if (ChunkRenderListSolidOffsetCache.count(getChunkID(SpreadPos.x + Pos.x - rd, SpreadPos.y + Pos.y - rd, SpreadPos.z + Pos.z - rd))) {
				ChunkRenderListSolidSorted.emplace(ChunkRenderListSolid[ChunkRenderListSolidOffsetCache[getChunkID(SpreadPos.x + Pos.x - rd, SpreadPos.y + Pos.y - rd, SpreadPos.z + Pos.z - rd)]]);
			}
		}*/

		std::deque<ChunkRenderDataBufferAddress> ChunkRenderListSolidSorted;

		for (const auto& e : ChunkRenderListSolid) {
			ChunkRenderListSolidSorted.emplace_back(e);
		}

		

		SolidChunkShaderPos.clear();

		//fr.CalculateFrustum(camera);

		int SolidIndex = 1;

		while (!ChunkRenderListSolidSorted.empty()) {
			
			ChunkRenderDataBufferAddress data = ChunkRenderListSolidSorted.front();
			ChunkRenderListSolidSorted.pop_front();

			if (!(data.x + -Pos.x > RenderDistance || data.y + -Pos.y > RenderDistance || data.z + -Pos.z > RenderDistance || data.x + -Pos.x < -RenderDistance || data.y + -Pos.y < -RenderDistance || data.z + -Pos.z < -RenderDistance)) {//if (FindDistance(data.second.x, data.second.y, data.second.z, (int)x12 / CHUNK_SIZE, (int)y12 / CHUNK_SIZE, (int)z12 / CHUNK_SIZE) <= renderDistance) {
			//	if (fr.SphereInFrustum((float)data.x * CHUNK_SIZE, (float)data.y * CHUNK_SIZE, (float)data.z * CHUNK_SIZE, (float)CHUNK_SIZE * 2)) {
					DrawArraysIndirectCommand cmd;
					cmd.count = data.size / (sizeof(unsigned int) * 2);
					cmd.instanceCount = 1;
					cmd.first = data.offset / (sizeof(unsigned int) * 2);
					cmd.baseInstance = SolidIndex;
					DrawArraysIndirectCommandListSolid.push_back(cmd);
					SolidChunkShaderPos.emplace_back(data.x);
					SolidChunkShaderPos.emplace_back(data.y);
					SolidChunkShaderPos.emplace_back(data.z);
					SolidIndex++;
			//	}
			}

			
			//std::cout << ChunkRenderListSolidSorted.size() << "\n";
		}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, SolidChunkShaderPos.size() * sizeof(int), SolidChunkShaderPos.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, IBO);
		glBufferSubData(GL_DRAW_INDIRECT_BUFFER,0, DrawArraysIndirectCommandListSolid.size() * sizeof(DrawArraysIndirectCommand), DrawArraysIndirectCommandListSolid.data());
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
	}

	

	void insertData(GLuint buffer, GLenum target, size_t offset, std::vector<unsigned int>* data) {
		glBindBuffer(target, buffer);
		glBufferSubData(target, offset, data->size() * sizeof(unsigned int), data->data());
		glBindBuffer(target, 0);
	}

	void draw() {

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glBindVertexArray(VAO);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, IBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBO);
		SolidShader->use();
	//	std::cout << DrawArraysIndirectCommandListSolid.size() << "\n";
		glMultiDrawArraysIndirect(GL_TRIANGLES, (GLvoid*)0, DrawArraysIndirectCommandListSolid.size(),0);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void UpdateData() {
		int width, height;

		glfwGetWindowSize(window, &width, &height);
		glm::mat4 model = glm::mat4(1.f);

		//camera->Position = vec3(0,49,0);
		glm::mat4 view = camera->GetViewMatrix();
		
		int x = width;
		int y = height;
		glm::mat4 projection = glm::perspective(glm::radians(camera->FOV), (float)x / (float)y, 0.1f, 1000000.0f);
		SolidShader->use();

		SolidShader->setMat4("view", view);
		SolidShader->setMat4("model", model);
		SolidShader->setMat4("projection", projection);
		SolidShader->setFloat("RenderDistance", (float)(RenderDistance * CHUNK_SIZE));
		SolidShader->setVec3("camPos", camera->Position);
		
		
		
	}

	void ReplaceChunk(Chunk chunk) {
		ChunkMesh Mesh;
		Mesh.chunk = &chunk;
		Mesh.SmartGreedyMeshing();

		if (MeshList.count(getChunkID(chunk.pos))) {
			DeleteChunk(getChunkID(chunk.pos));
		}

		AddChunk(&Mesh);
	}

	void AddChunk(Chunk chunk) {
		if (MeshList.count(getChunkID(chunk.pos))) {
			ReplaceChunk(chunk);
			return;
		}

		ChunkMesh Mesh;
		Mesh.chunk = &chunk;
		Mesh.SmartGreedyMeshing();

		AddChunk(&Mesh);
	}

	void ReloadAssets() {
		std::vector<unsigned char> data;
		std::vector<std::string> filenames =
		{
			"dirt.png",
			"grass_block_side.png",
			"grass_block_top.png",
			"stone.png",
			"sand.png",
			"water_still.png",
			"oak_log.png",
			"oak_log_top.png",
			"oak_leaves.png",
			"oak_sapling.png"
		};

		int width, height, nrComponents;

		for (int i = 0; i < filenames.size(); i++) {
			std::string path = "assets/textures/array/block/" + filenames.data()[i];
			unsigned char* tdata = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
			std::cout << "Width: " << width << " Height: " << height << "nrComponents: " << nrComponents << " Loaded Image: " << path << "\n";
			if (tdata) {
				if (nrComponents == 3) {
					for (int index = 0; index < width * height; index++) {
						data.push_back(tdata[(index * nrComponents)]);
						data.push_back(tdata[(index * nrComponents) + 1]);
						data.push_back(tdata[(index * nrComponents) + 2]);
						data.push_back(255);
					}
				}
				else if (nrComponents == 4) {
					for (int index = 0; index < width * height; index++) {
						data.push_back(tdata[(index * nrComponents)]);
						data.push_back(tdata[(index * nrComponents) + 1]);
						data.push_back(tdata[(index * nrComponents) + 2]);
						data.push_back(tdata[(index * nrComponents) + 3]);
					}
				}

			}
			else {
				getLogger()->LogError("Chunk Renderer", "Unable to load texture: " + path);
			}

		}


		GLsizei layerCount = (GLsizei)filenames.size();
		GLsizei mipLevelCount = 5;

		// Read you texels here. In the current example, we have 2*2*2 = 8 texels, with each texel being 4 GLubytes.


		glGenTextures(1, &BlockTexture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, BlockTexture);
		// Allocate the storage.
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGBA8, 16, 16, layerCount);
		// Upload pixel data.
		// The first 0 refers to the mipmap level (level 0, since there's only 1)
		// The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
		// The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
		// Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, 16, 16, layerCount, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
		//glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, layerCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
		//glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		
		// Always set reasonable texture parameters


		SolidShader->bindTextureArray2D(0, BlockTexture, "BlockTexture");
	}

	std::unordered_map<long long int, bool> MeshList;


	//Settings

	int RenderDistance = 10;
	glm::vec3 PlayerPosition = glm::vec3(0.f,0.f,0.f);


private:

	std::vector<glm::vec3> SpreadCache;
	int SpreadCacheRenderDistance = 0;
	int SpreadCacheRenderHeight = 0;

	std::deque<int> FIBO;
	std::vector<int> spread;

	GLuint VBO, VAO, IBO, SSBO = 0;
	GLuint VBOT, VAOT, IBOT = 0;

	GLuint BlockTexture = 0;

	Shader* SolidShader = nullptr;

	std::vector<GLint> SolidChunkShaderPos;
	std::vector<GLint> TransparentChunkShaderPos;

	size_t GPUBufferSizeSolid = 250000000;
	size_t GPUBufferSizeTransparent = 250000000;
	size_t GPUSSBOMAXSIZE = 1000000;

	GLFWwindow* window = nullptr;
	Camera* camera = nullptr;
	CFrustum fr;

	std::unordered_map<long long int, int> ChunkRenderListSolidOffsetLookup;
	std::unordered_map<long long int, int> ChunkRenderListTransparentOffsetCache;

	std::vector<ChunkRenderDataBufferAddress> ChunkRenderListSolid;
	std::vector<DrawArraysIndirectCommand> DrawArraysIndirectCommandListSolid;

	std::vector<ChunkRenderDataBufferAddress> ChunkRenderListTransparent;
	std::vector<DrawArraysIndirectCommand> DrawArraysIndirectCommandListTransparent;
};


#endif
/*
===============================================================
===============================================================
*/

#include "precompiled.hpp"
#include "md5Model.hpp"

#include <SDL3/SDL_endian.h>

md5Model::md5Model( void )
{
}

md5Model::~md5Model( void )
{
}

// Função auxiliar pra dividir linha
static std::vector<std::string> split(const std::string &s) 
{
	
    std::stringstream ss(s);
    std::vector<std::string> tokens;
    std::string t;
    while (ss >> t) 
		tokens.push_back(t);
    return tokens;
}

bool md5Model::Load(const std::string in_path)
{
	uint32_t numJoints = 0;
	uint32_t numMeshes = 0;
    std::string line;

	std::ifstream file( in_path );
    if (!file.is_open()) 
	{
        std::cerr << "Erro: não foi possível abrir " << in_path << std::endl;
        return false;
    }

    while ( std::getline(file, line) ) 
	{
        if (line.empty()) 
			continue;

        if ( line.find("numJoints") != std::string::npos) 
		{
            numJoints = std::stoi(split(line)[1]);
			m_joints.resize( numJoints );
        } 
		else if (line.find("numMeshes") != std::string::npos) 
		{
            numMeshes = std::stoi(split(line)[1]);
			m_meshes.resize( numMeshes );
        } 
		else if ( line.find("joints") != std::string::npos )
		{	
			uint32_t currentJoint = 0;
			while ( std::getline(file, line ) )
			{
				int parent = 0;
				std::string name;
				glm::vec3 pos;
				glm::quat orient;

				if (line.empty() || line.find("{") != std::string::npos )
					continue;

				if (line.find("}") != std::string::npos)
					break;

				// Exemp: "bone1" 0 ( 10 0 0 ) ( 0.707 0 0.707 )
				
            	std::stringstream ss(line);
            	ss >> std::quoted( name ) >> parent;
            	char c; // ignore ( & )
            	ss >> c >> pos.x >> pos.y >> pos.z >> c; // (pos)
            	ss >> c >> orient.x >> orient.y >> orient.z >> c; // (orient)

				m_joints[currentJoint].Configure( currentJoint, name, &m_joints[parent] );
				m_joints[currentJoint].Transfom( pos, orient, true );
				currentJoint++;
				}
        }
        else if (line.find("mesh") != std::string::npos) 
		{

			auto currentMesh = m_meshes.begin(); 
			while ( std::getline(file, line) )
			{
				if (line.empty()) 
					continue;
				 
				// get the mesh shader 
				if ( line.find("shader") != std::string::npos ) 
				{
					std::stringstream ss(line);
					std::string shader;
					ss >> std::quoted( shader );
					currentMesh->SetShader( shader );
				}
				// get the vertex array 
				else if ( line.find("numverts") != std::string::npos )
				{
					uint32_t numverts = std::stoi(split(line)[1]);	
					currentMesh->ResizeVetices( numverts );

					for ( uint32_t i = 0; i < numverts; i++)
					{
						int index = 0;
						int32_t start = 0;
						int32_t count = 0;
						glm::vec2 st( 0.0f );
						std::stringstream ss(line);
						std::string dummy;
						char c;

						// get a vertex line
						std::getline( file, line );

						// we don't found a valid vertex 
						if ( line.find("vert") == std::string::npos )
							break;

						ss >> dummy >> index;
						ss >> c >> st.x >> st.y >> c;
						ss >> start >> count;
						currentMesh->GetVertices()[i] = md5Vertex( start, count, st );
					}
				}
				else if ( line.find("numtris") != std::string::npos )
				{
					uint32_t numtris = std::stoi(split(line)[1]);	
					currentMesh->ResizeTriangles( numtris );

					for ( uint32_t i = 0; i < numtris; i++)
					{
						int32_t index = 0;
						md5Triangle tri;
						// get a triangle line
						std::getline( file, line );

						// we don't found a valid triangle 
						if ( line.find("tri") == std::string::npos )
							break;

						//Triangle t;
						std::stringstream ss(line);
						std::string dummy;
						ss >> dummy >> index >> tri.v1 >> tri.v2 >> tri.v3;
						currentMesh->GetTriangles()[i] = tri;						 
					}
				}
				else if ( line.find("numweights") != std::string::npos )
				{
					uint32_t numweights = std::stoi(split(line)[1]);	
					currentMesh->ResizeWeights( numweights );

					for ( uint32_t i = 0; i < numweights; i++)
					{	
						char c;
						int32_t index = 0;
						int32_t joint = 0;
						float	bias = 0.0f;
						glm::vec3 pos;
						std::stringstream ss(line);
						std::string dummy;
						
						// get a triangle line
						std::getline( file, line );

						if (line.find("weight") == std::string::npos) 
							break;
						
						ss >> dummy >> index >> joint >> bias;
						ss >> c >> pos.x >> pos.y >> pos.z >> c;
						currentMesh->GetWeights()[i] = md5Weight( joint, bias, pos );
					}
				}
				else if (line.find("}") != std::string::npos )
				{
					currentMesh++;
					break;
				}
			}
        }  
	
    }

    return true;
}

bool md5Model::Save(const std::string in_path)
{
	uint32_t i = 0;
	uint32_t j = 0;
	std::ofstream file( in_path );
    if (!file.is_open()) 
	{
        std::cerr << "Erro: não foi possível abrir " << in_path << std::endl;
        return false;
    }

	// write file version
	file << "MD5Version " << VERSION << "\n";

	// write commandline
	file << "commandline " << "\"" << m_comandLine << "\"\n\n";

	// write the joints and meshes count
	file << "numJoints " << m_joints.size() << "\n";
	file << "numMeshes " << m_meshes.size() << "\n\n";

	file << "joints\n{\n";

	for ( i = 0; i < m_joints.size(); i++)
	{
		md5Joint joint = m_joints[i];
		glm::vec3 pos = joint.GetPosition();
		glm::quat ori = joint.GetOrientation();

		// write joint "name" and parent index ( -1 for root )
		file << "\t\"" << joint.Name() << "\" " << joint.GetParentIndex();
		
		// write joint position
		file << " ( " << pos.x << " " << pos.y << " " << pos.z << " ) ";
		
		// write joint orientation
		file << " ( " << ori.x << " " << ori.y << " " << ori.z << " )\n";
	}

	file << "}\n";
	file << std::endl;

	for ( i = 0; i < m_meshes.size(); i++)
	{
		md5Mesh mesh = m_meshes[i];
		file << "mesh\n{\n";

		file << "\tshader \"" << mesh.Shader() << "\"\n\n";

		file << "\tnumverts " << mesh.NumVertices() << "\n";
		for ( j = 0; j < mesh.NumVertices(); j++ )
		{
			auto vert = mesh.GetVertices()[j];
			glm::vec2 uv = vert.TextCoord();
			file << "\tvert " << j << " ( " << uv.x << " " << uv.y << " ) " << vert.WeightStart() << " " << vert.WeightCount() << "\n";
		}
		file << std::endl;

		file << "\tnumtris " << mesh.NumTriangles() << "\n";
		for ( j = 0; j < mesh.NumTriangles(); j++)
		{
			auto tri = mesh.GetTriangles()[j];
			file << "\ttri " << j << " " << tri.v1 << " " << tri.v2 << " " << tri.v3 << "\n";
		}
		file << std::endl;


		file << "\tnumweights " << mesh.NumWeights();
		for ( j = 0; j < mesh.NumWeights(); j++ )
		{
			auto weight = mesh.GetWeights()[j];
			auto pos = weight.Position();
			
			// write weight index, joint id, bias
			file << "\tweight " << j << " " << weight.Joint() << " " << weight.Bias() << " ";

			// write joint related position
			file << "( " << pos.x << " " << pos.y << " " << pos.z << " )\n"; 
		}
		file << std::endl;

		// mesh end 
		file << "}"<< std::endl;
	}

	file.close();
    return true;
}

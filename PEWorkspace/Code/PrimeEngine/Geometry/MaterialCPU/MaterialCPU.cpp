#define NOMINMAX

// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Immediate include
#include "MaterialCPU.h"

// Outer-Engine includes

// Inter-Engine includes

#include "PrimeEngine/FileSystem/FileReader.h"
#include "PrimeEngine/Utils/StringOps.h"
#include "PrimeEngine/MainFunction/MainFunctionArgs.h"
// Sibling/Children includes

namespace PE {

MaterialCPU::MaterialCPU(PE::GameContext &context, PE::MemoryArena arena)
: m_textureFilenames(context, arena)
, m_texturePackages(context, arena)
, m_textureFamilies(context, arena)
, m_customSamplerStates(context, arena)
{
	m_arena = arena; m_pContext = &context;
	m_dbgName[0] = '\0';
}

// Reads the specified buffer from file
void MaterialCPU::ReadMaterial(const char *filename, const char *package)
{
	if (StringOps::length(filename) < sizeof(m_dbgName))
	{
		StringOps::writeToString(filename, m_dbgName, sizeof(m_dbgName));
	}

    PEString::generatePathname(*m_pContext, filename, package, "Materials", PEString::s_buf, PEString::BUF_SIZE);
	
	// Path is now a full path to the file with the filename itself
	FileReader f(PEString::s_buf);

	char line[256];
	f.nextNonEmptyLine(line, 255);
	// TODO : make sure it is "MATERIAL"
	int version = 0;
	if (0 == StringOps::strcmp(line, "MATERIAL_V1"))
		version = 1;

	// diffuse
	PrimitiveTypes::Float32 dr, dg, db, da; 
	f.nextFloat32(dr);
	f.nextFloat32(dg);
	f.nextFloat32(db);
	f.nextFloat32(da);
	m_diffuse = Vector4(dr, dg, db, da);

	// shininess
	PrimitiveTypes::Float32 shininess;
	f.nextFloat32(shininess);
	m_shininess = shininess;
	if (version == 1)
	{
		PrimitiveTypes::Float32 shininess1;
		f.nextFloat32(shininess1);
		PrimitiveTypes::Float32 shininess2;
		f.nextFloat32(shininess2);

		m_shininess = (shininess+shininess1+shininess2)/3;
	}

	// specular
	PrimitiveTypes::Float32 sr, sg, sb;
	f.nextFloat32(sr);
	f.nextFloat32(sg);
	f.nextFloat32(sb);
	m_specular = Vector3(sr, sg, sb);
	
	// emissive
	PrimitiveTypes::Float32 er, eg, eb;
	f.nextFloat32(er);
	f.nextFloat32(eg);
	f.nextFloat32(eb);
	m_emissive = Vector3(er, eg, eb);

	PrimitiveTypes::Int32 nTextures;
	f.nextInt32(nTextures);
	m_textureFilenames.reset(nTextures);
	m_texturePackages.reset(nTextures);
	m_textureFamilies.reset(nTextures);
	m_customSamplerStates.reset(nTextures);

	for (PrimitiveTypes::Int32 iTex = 0; iTex < nTextures; iTex++)
	{
		char texType[256];
		f.nextNonEmptyLine(texType, 255);
		char texFilename[256];
		f.nextNonEmptyLine(texFilename, 255);
		PEString sTexFilename(*m_pContext, m_arena);
		sTexFilename.append(texFilename);
		m_textureFilenames.add(sTexFilename);
		PEString sTexPackage(*m_pContext, m_arena);
		sTexPackage.append(package);
		m_texturePackages.add(sTexPackage);

		ESamplerState samplerState = SamplerState_Count;
		m_customSamplerStates.add(samplerState);

		if (StringOps::strcmp(texType, "COLOR") == 0)
		{
			// this is a color map
			m_textureFamilies.add(TextureFamily::COLOR_MAP);
		}
		else if (StringOps::strcmp(texType, "COLOR_CUBE") == 0)
		{
			// this is a bump map
			m_textureFamilies.add(TextureFamily::COLOR_CUBE);
		}
		else if (StringOps::strcmp(texType, "BUMP") == 0)
		{
			// this is a bump map
			m_textureFamilies.add(TextureFamily::BUMP_MAP);
		}
		else if (StringOps::strcmp(texType, "SPECULAR") == 0)
		{
			// this is a specular map
			m_textureFamilies.add(TextureFamily::SPECULAR_MAP);
		}
		else if (StringOps::strcmp(texType, "GLOW") == 0)
		{
			// this is a specular map
			m_textureFamilies.add(TextureFamily::GLOW_MAP);
		}
	}
}

void MaterialCPU::createDefaultMaterial()
{
	m_diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

	// shininess
	m_shininess = 0.0f;

	// specular
	m_specular = Vector3(0.0f, 0.0f, 0.0f);
	
	// emissive
	m_emissive = Vector3(0.0f, 0.0f, 0.0f);

	m_textureFilenames.reset(0);
	m_texturePackages.reset(0);
	m_textureFamilies.reset(0);
	m_customSamplerStates.reset(0);
}

void MaterialCPU::createMaterialWithColorTexture(const char *textureFilename, const char *package, ESamplerState customSamplerState /* = SamplerState_Count*/)
{
	m_diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

	// shininess
	m_shininess = 0.0f;

	// specular
	m_specular = Vector3(0.0f, 0.0f, 0.0f);
	
	// emissive
	m_emissive = Vector3(0.0f, 0.0f, 0.0f);

	m_textureFilenames.reset(1);
	m_texturePackages.reset(1);
	m_textureFamilies.reset(1);
	m_customSamplerStates.reset(1);

    PEString sTexFilename(*m_pContext, m_arena);
	sTexFilename.append(textureFilename);
	m_textureFilenames.add(sTexFilename);
	PEString sPackage(*m_pContext, m_arena);
	sPackage.append(package);
	m_texturePackages.add(sPackage);

	// this is a color map
	m_textureFamilies.add(TextureFamily::COLOR_MAP);

	m_customSamplerStates.add(customSamplerState);
}

void MaterialCPU::createMaterialWithOneColorTextureArrayMaterial(const char * filenames[], PrimitiveTypes::UInt32 nFilenames, const char *package)
{
	m_diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

	// shininess
	m_shininess = 0.0f;

	// specular
	m_specular = Vector3(0.0f, 0.0f, 0.0f);
	
	// emissive
	m_emissive = Vector3(0.0f, 0.0f, 0.0f);

	m_textureFilenames.reset(nFilenames);
	m_textureFamilies.reset(nFilenames);
	m_texturePackages.remove(nFilenames);

	for (PrimitiveTypes::UInt32 it = 0; it < nFilenames; ++it)
	{
		PEString sTexFilename(*m_pContext, m_arena);
		sTexFilename.append(filenames[it]);
		m_textureFilenames.add(sTexFilename);

		PEString sPacakge(*m_pContext, m_arena);
		sPacakge.append(package);
		m_texturePackages.add(sPacakge);
		// this is a color map
		m_textureFamilies.add(TextureFamily::COLOR_MAP_ARRAY);
		m_customSamplerStates.add(SamplerState_Count); // will end up suing default color texture sampler state
	}
}


void MaterialCPU::createMaterialWithColorAndGlowTextures(const char *colorTextureFilename, const char *glowTextureFilename, const char *package)
{
	m_diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

	// shininess
	m_shininess = 0.0f;

	// specular
	m_specular = Vector3(0.0f, 0.0f, 0.0f);
	
	// emissive
	m_emissive = Vector3(0.0f, 0.0f, 0.0f);

	m_textureFilenames.reset(2);
	m_textureFamilies.reset(2);
	m_texturePackages.reset(2);
	m_customSamplerStates.reset(2);


	PEString sTexFilename(*m_pContext, m_arena);
	sTexFilename.append(colorTextureFilename);
	m_textureFilenames.add(sTexFilename);

	PEString sPackage(*m_pContext, m_arena);
	sPackage.append(package);
	m_texturePackages.add(sPackage);

	// this is a color map
	m_textureFamilies.add(TextureFamily::COLOR_MAP);
	m_customSamplerStates.add(SamplerState_Count); // will end up suing default color texture sampler state

	PEString sGlowTexFilename(*m_pContext, m_arena);
	sGlowTexFilename.append(glowTextureFilename);
	m_textureFilenames.add(sGlowTexFilename);
	
	PEString sGlowPackage(*m_pContext, m_arena);
	sPackage.append(package);
	m_texturePackages.add(sGlowPackage);

	// this is a color map
	m_textureFamilies.add(TextureFamily::GLOW_MAP);
	m_customSamplerStates.add(SamplerState_Count); // will end up suing default glow texture sampler state

}

}; // namespace PE

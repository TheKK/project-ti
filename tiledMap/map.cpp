#include "map.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <SDL2/SDL_image.h>

#include "controller.h"
#include "camera.h"

SDL_Texture*
loadTexture_(SDL_Renderer* renderer, const std::string& filePath)
{
	SDL_Surface* loadedImage = nullptr;
	SDL_Texture* tex = nullptr;
	char* basePath = nullptr;
	std::string fullPath;

	basePath = SDL_GetBasePath();
	fullPath += basePath;
	fullPath += filePath;

	SDL_free(basePath);
	basePath = nullptr;

	loadedImage = IMG_Load(fullPath.c_str());
	if (loadedImage == nullptr) {
		std::string errMsg("IMG error while opening: ");
		errMsg += IMG_GetError();
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, errMsg.c_str());
		return nullptr;
	}

	tex = SDL_CreateTextureFromSurface(renderer, loadedImage);
	if (tex == nullptr) {
		std::string errMsg("SDL error while converting surface: ");
		errMsg += SDL_GetError();
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, errMsg.c_str());
		return nullptr;
	}

	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

	SDL_FreeSurface(loadedImage);

	return tex;
}

MapLoader::MapLoader()
{
}

MapLoader::MapLoader(const std::string& filePath)
{
	load(filePath);
}

MapLoader::~MapLoader()
{
}

void
MapLoader::load(const std::string& filePath)
{
	Json::Reader reader;
	std::fstream fd;

	fd.open(filePath, std::fstream::in);
	if (!fd.is_open())
		throw std::runtime_error("file oepn error");

	if (reader.parse(fd, mapData_) == false) {
		fd.close();
		throw std::runtime_error(reader.getFormattedErrorMessages());
	}

	fd.close();
}

const Json::Value&
MapLoader::getLayer(const std::string& layerName) const
{
	if (mapData_["layers"].isNull())
		throw std::runtime_error(
			"No such object 'layers' in map file, it's borken");

	for (auto& layer : mapData_["layers"]) {
		if (layer["name"].asString() == layerName)
			return layer;
	}

	throw std::runtime_error("No such layer in map file");
}

const Json::Value&
MapLoader::getTileSets() const
{
	const Json::Value& tileSetsToReturn = mapData_["tilesets"];

	if (tileSetsToReturn.isNull())
		throw std::runtime_error(
			"No such object 'tilesets' in map file, it's borken");

	return tileSetsToReturn;
}

MapTileLayer::MapTileLayer()
{
}

MapTileLayer::MapTileLayer(SDL_Renderer* renderer, const MapLoader& mapLoader,
			   const std::string& layerName)
{
	load(renderer, mapLoader, layerName);
}

MapTileLayer::~MapTileLayer()
{
	cleanUp();
}

void
MapTileLayer::load(SDL_Renderer* renderer, const MapLoader& mapLoader,
		   const std::string& layerName)
{
	layer_ = mapLoader.getLayer(layerName);

	for (auto& tileSet : mapLoader.getTileSets()) {
		std::string imageName = tileSet["image"].asString();

		tileSets_.push_back(tileSet);
		tileImages_[imageName] = loadTexture_(renderer, imageName);
		if (!tileImages_[imageName])
			throw std::runtime_error("file not found");

	}

	/* Order from large to small, 10 , 5 , 3, 2 */
	std::sort(tileSets_.begin(), tileSets_.end(),
		  [](const Json::Value& a, const Json::Value& b)
		  {
			  int gidA = a["firstgid"].asInt();
			  int gidB = b["firstgid"].asInt();

			  return gidA > gidB;
		  });
}

void
MapTileLayer::cleanUp()
{
	layer_.clear();
	tileSets_.clear();
		
	for (auto& e : tileImages_) {
		SDL_DestroyTexture(e.second);
		e.second = nullptr;
	}
	tileImages_.clear();
}

void
MapTileLayer::update()
{
}

void
MapTileLayer::render(SDL_Renderer* renderer, const Camera& camera)
{
	int mapWidth, mapHeight;
	const SDL_Rect& cameraRect = camera.getViewRect();

	mapWidth = layer_["width"].asInt();
	mapHeight = layer_["height"].asInt();

	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			int startTileId, tileId;
			int tileWidth, tileHeight;
			int tileRow, tileCol;
			SDL_Texture* tileImage = nullptr;
			SDL_Rect tileClip, tileDst;

			tileId = layer_["data"][mapWidth * y + x].asInt();
			if (tileId == 0)
				continue;

			/* Find out sprite sheet */
			for (auto& tileSet : tileSets_) {
				startTileId = tileSet["firstgid"].asInt();
				if (tileId < startTileId)
					continue;

				tileImage =
					tileImages_[tileSet["image"]
					.asString()];

				tileWidth = tileSet["tilewidth"].asInt();
				tileHeight = tileSet["tileheight"].asInt();

				tileRow =
					tileSet["imageheight"].asInt() / 
					tileSet["tileheight"].asInt();
				tileCol =
					tileSet["imagewidth"].asInt() / 
					tileSet["tilewidth"].asInt();
				break;
			}

			/* Find sprite pos */
			for (int row = 0; row < tileRow; ++row) {
				for (int col = 0; col < tileCol; ++col) {
					if ((tileId - startTileId) !=
					    (row * tileCol + col))
						continue;

					tileClip.x = col * tileWidth;
					tileClip.y = row * tileHeight;
					tileClip.w = tileWidth;
					tileClip.h = tileHeight;
				}
			}

			tileDst.x = x * tileWidth - cameraRect.x;
			tileDst.y = y * tileHeight - cameraRect.y;
			tileDst.w = tileWidth;
			tileDst.h = tileHeight;

			SDL_RenderCopy(renderer, tileImage, &tileClip,
				       &tileDst);
		}
	}
}

int
MapTileLayer::getMapWidth() const
{
	return layer_["width"].asInt();
}

int
MapTileLayer::getMapHeight() const
{
	return layer_["height"].asInt();
}

int
MapTileLayer::getTileWidth() const
{
	return layer_["tilewidth"].asInt();
}

int
MapTileLayer::getTileHeight() const
{
	return layer_["tileheight"].asInt();
}

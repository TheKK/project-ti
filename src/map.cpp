#include "map.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <SDL2/SDL_image.h>

#include "controller.h"
#include "camera.h"
#include "graphics.h"

/* =================================== MapLoader */
MapLoader::MapLoader()
{
}

MapLoader::MapLoader(const std::string& filePath)
{
	load(filePath);
}

void
MapLoader::load(const std::string& filePath)
{
	Json::Reader reader;
	std::fstream fd;

	fd.open(filePath, std::fstream::in);
	if (!fd.is_open()) {
		std::string errMsg = "[MapLoader] file not found: " + filePath;

		throw std::runtime_error(errMsg);
	}

	if (reader.parse(fd, mapData_) == false) {
		fd.close();
		throw std::runtime_error(reader.getFormattedErrorMessages());
	}

	mapName_ = filePath;
}

const Json::Value&
MapLoader::getTileLayer(const std::string& layerName) const
{
	if (mapData_["layers"].isNull())
		throw std::runtime_error("No object 'layers' in map file, "
					 "map file '" + mapName_ +
					 "' is broken");

	for (auto& layer : mapData_["layers"]) {
		if ((layer["name"].asString() == layerName) &&
		    (layer["type"].asString() == "tilelayer"))
			return layer;
	}

	throw std::runtime_error("No tile layer named '" + layerName +
				 "' in map file: " + mapName_);
}

const Json::Value&
MapLoader::getObjectLayer(const std::string& layerName) const
{
	if (mapData_["layers"].isNull())
		throw std::runtime_error("No object 'layers' in map file, "
					 "map file '" + mapName_ +
					 "' is broken");

	for (auto& layer : mapData_["layers"]) {
		if ((layer["name"].asString() == layerName) &&
		    (layer["type"].asString() == "objectgroup"))
			return layer;
	}

	throw std::runtime_error("No object layer named '" + layerName +
				 "' in map file: " + mapName_);
}

const Json::Value&
MapLoader::getTileSets() const
{
	const Json::Value& tileSetsToReturn = mapData_["tilesets"];

	if (tileSetsToReturn.isNull())
		throw std::runtime_error("No object 'tilesets' in map file, "
					 "map file '" + mapName_ +
					 "' is broken");

	return tileSetsToReturn;
}

int
MapLoader::getTileWidth() const
{
	char objectName[] = "tilewidth";

	SDL_assert (!mapData_[objectName].isNull());

	return mapData_[objectName].asInt();
}

int
MapLoader::getTileheight() const
{
	char objectName[] = "tilewidth";

	SDL_assert(!mapData_[objectName].isNull());

	return mapData_[objectName].asInt();
}

/* =================================== MapTileLayer */
MapTileLayer::MapTileLayer()
{
}

MapTileLayer::MapTileLayer(Graphics& graphics, const MapLoader& mapLoader,
			   const std::string& layerName)
{
	load(graphics, mapLoader, layerName);
}

MapTileLayer::MapTileLayer(const MapTileLayer& clone)
{
	this->cleanUp();

	this->layer_ = clone.layer_;

	this->mapWidth_ = clone.mapWidth_;
	this->mapHeight_ = clone.mapHeight_;
	this->tileWidth_ = clone.tileWidth_;
	this->tileHeight_ = clone.tileHeight_;

	this->tileSets_  = clone.tileSets_;
	this->tileSprites_ = clone.tileSprites_;
}

MapTileLayer::~MapTileLayer()
{
	cleanUp();
}

void
MapTileLayer::load(Graphics& graphics, const MapLoader& mapLoader,
		   const std::string& layerName)
{
	/*
	 * Here we suppose each tileset used by this tilelayer are all the
	 * same size (tileWidth, tileHeight, etc).
	 */

	layer_ = mapLoader.getTileLayer(layerName);

	tileWidth_ = mapLoader.getTileWidth();
	tileHeight_ = mapLoader.getTileheight();

	mapWidth_ = layer_["width"].asInt();
	mapHeight_ = layer_["height"].asInt();

	for (auto& tileSet : mapLoader.getTileSets())
		tileSets_.push_back(tileSet);

	/* "firstgid" order from large to small, 10 , 5 , 3, 2 */
	std::sort(tileSets_.begin(), tileSets_.end(),
		  [](const Json::Value& a, const Json::Value& b)
		  {
			  int gidA = a["firstgid"].asInt();
			  int gidB = b["firstgid"].asInt();

			  return gidA > gidB;
		  });

	/* Find out sprite sheet */
	for (auto& tileSet : tileSets_) {
		int tileRow, tileCol;
		SDL_Rect tileClip;
		std::string imageName;

		imageName = tileSet["image"].asString();

		tileRow =
			tileSet["imageheight"].asInt() /
			tileSet["tileheight"].asInt();
		tileCol =
			tileSet["imagewidth"].asInt() /
			tileSet["tilewidth"].asInt();

		/* Find sprite pos */
		for (int row = 0; row < tileRow; ++row) {
			for (int col = 0; col < tileCol; ++col) {
				tileClip.x = col * tileWidth_;
				tileClip.y = row * tileHeight_;
				tileClip.w = tileWidth_;
				tileClip.h = tileHeight_;

				tileSprites_[imageName].emplace_back(
					graphics, imageName, tileClip);
			}
		}
	}
}

void
MapTileLayer::cleanUp()
{
	tileSets_.clear();
	tileSprites_.clear();
}

void
MapTileLayer::update()
{
}

void
MapTileLayer::render(Graphics& graphics, const Camera& camera)
{
	int leftBorder = std::floor((float)camera.getLeft() /
				    (float)tileWidth_);
	int rightBorder = std::floor((float)camera.getRight() /
				     (float)tileWidth_);
	int topBorder = std::floor((float)camera.getTop() /
				   (float)tileHeight_);
	int bottomBorder = std::floor((float)camera.getBottom() /
				      (float)tileHeight_);

	for (int y = topBorder; y <= bottomBorder; ++y) {
		for (int x = leftBorder; x <= rightBorder; ++x) {
			int tileId, firstTileId;

			tileId = layer_["data"][mapWidth_ * y + x].asInt();
			if (tileId == 0)
				continue;

			for (const auto& tileSet : tileSets_) {
				SDL_Rect tileDst;
				std::string imageName;

				firstTileId = tileSet["firstgid"].asInt();
				if (tileId < firstTileId)
					continue;

				imageName = tileSet["image"].asString();

				tileDst.x = x * tileWidth_ - camera.getLeft();
				tileDst.y = y * tileHeight_ - camera.getTop();
				tileDst.w = tileWidth_;
				tileDst.h = tileHeight_;

				tileSprites_[imageName][tileId - firstTileId]
					.render(graphics, &tileDst);

				break;
			}
		}
	}
}

std::vector<SDL_Rect>
MapTileLayer::getCollidedTiles(const SDL_Rect& rect) const
{
	std::vector<SDL_Rect> tilesToReturn;
	int leftBorder, rightBorder, topBorder, bottomBorder;

	leftBorder = std::floor(rect.x / tileWidth_);
	rightBorder = std::ceil((rect.x + rect.w - 1) / tileWidth_);
	topBorder = std::floor(rect.y / tileHeight_);
	bottomBorder = std::ceil((rect.y + rect.h - 1) / tileHeight_);

	for (int row = topBorder; row <= bottomBorder; ++row) {
		for (int col = leftBorder; col <= rightBorder; ++col) {
			/* TODO
			 * Now return only solid tile, maybe not the
			 * best way
			 */
			if (layer_["data"][row * mapWidth_ + col] == 0)
				continue;

			SDL_Rect tile;

			tile.w = tileWidth_;
			tile.h = tileHeight_;
			tile.x = tileWidth_ * col;
			tile.y = tileHeight_ * row;

			tilesToReturn.push_back(tile);
		}
	}

	return tilesToReturn;
}

int
MapTileLayer::getMapWidth() const
{
	return mapWidth_;
}

int
MapTileLayer::getMapHeight() const
{
	return mapHeight_;
}

int
MapTileLayer::getTileWidth() const
{
	return tileWidth_;
}

int
MapTileLayer::getTileHeight() const
{
	return tileHeight_;
}

/* =================================== MapObjectLayer */
MapObjectLayer::MapObjectLayer()
{
}

MapObjectLayer::MapObjectLayer(const MapLoader& mapLoader,
			       const std::string& layerName)
{
	load(mapLoader, layerName);
}

MapObjectLayer&
MapObjectLayer::operator=(const Json::Value& target)
{
	this->layer_ = target;

	return *this;
}

Json::Value::iterator
MapObjectLayer::begin()
{
	return layer_["objects"].begin();
}

Json::Value::iterator
MapObjectLayer::end()
{
	return layer_["objects"].end();
}

void
MapObjectLayer::load(const MapLoader& mapLoader,
		     const std::string& layerName)
{
	layer_ = mapLoader.getObjectLayer(layerName);
}

void
MapObjectLayer::cleanUp()
{
	layer_.clear();
}

const Json::Value&
MapObjectLayer::getObject(const std::string& name)
{
	for (const Json::Value& e : layer_["objects"]) {
		if (e["name"].asString() == name)
			return e;
	}

	throw std::runtime_error("No object named '" + name + "' in layer '" +
				 layer_["name"].asString() + "'");
}
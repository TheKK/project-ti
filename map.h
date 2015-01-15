#ifndef MAP_H
#define MAP_H

#include <string>
#include <map>
#include <json/json.h>
#include <SDL2/SDL.h>

class Camera;
class Controller;

class MapLoader;
class MapTileLayer;

class MapLoader
{
public:
	MapLoader();
	MapLoader(const std::string& filePath);

	void load(const std::string& filePath);

	const Json::Value& getTileLayer(const std::string& layerName) const;
	const Json::Value& getObjectLayer(const std::string& layerName) const;
	const Json::Value& getTileSets() const;

	int getTileWidth() const;
	int getTileheight() const;
private:
	Json::Value mapData_;
};

class MapTileLayer
{
public:
	MapTileLayer();
	MapTileLayer(SDL_Renderer* renderer, const MapLoader& mapLoader,
		     const std::string& layerName);
	MapTileLayer(const MapTileLayer& clone);
	~MapTileLayer();

	void load(SDL_Renderer* renderer, const MapLoader& mapLoader,
		  const std::string& layerName);
	void cleanUp();

	void update();
	void render(SDL_Renderer* SDL_Renderer, const Camera& camera);

	std::vector<SDL_Rect> getCollidedTiles(const SDL_Rect& rect) const;

	int getMapWidth() const;
	int getMapHeight() const;
	int getTileWidth() const;
	int getTileHeight() const;
private:
	Json::Value layer_;

	int mapWidth_, mapHeight_;
	int tileWidth_, tileHeight_;

	std::vector<Json::Value> tileSets_;
	std::map<std::string, SDL_Texture*> tileImages_;
};

class MapObjectLayer
{
public:
	MapObjectLayer();
	MapObjectLayer(const MapLoader& mapLoader,
		       const std::string& layerName);

	MapObjectLayer& operator=(const Json::Value& target);

	void load(const MapLoader& mapLoader,
		  const std::string& layerName);
	void cleanUp();

	const Json::Value& getObject(const std::string& name);
private:
	Json::Value layer_;
};

#endif /* MAP_H */


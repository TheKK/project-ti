#ifndef MAP_H
#define MAP_H

#include <string>
#include <map>
#include <jsoncpp/json/json.h>
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
	~MapLoader();

	void load(const std::string& filePath);

	const Json::Value& getTileLayer(const std::string& layerName);
	const Json::Value& getTileSets();
private:
	Json::Value mapData_;
};

class MapTileLayer
{
public:
	MapTileLayer();
	MapTileLayer(SDL_Renderer* renderer,
		     const Json::Value& layer,
		     const Json::Value& tileSets);
	~MapTileLayer();

	void load(SDL_Renderer* renderer,
		  const Json::Value& layer,
		  const Json::Value& tileSets);
	void cleanUp();

	void update();
	void render(SDL_Renderer* SDL_Renderer, const Camera& camera);

	int getMapWidth() const;
	int getMapHeight() const;
	int getTileWidth() const;
	int getTileHeight() const;
private:
	Json::Value layer_;

	std::vector<Json::Value> tileSets_;
	std::map<std::string, SDL_Texture*> tileImages_;
};

#endif /* MAP_H */


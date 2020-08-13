#include "TileLayer.h"
#include <algorithm>
#include <string>

TileLayer::TileLayer(const char* layerName,const char* tilesetKey,Texture2D& texture, int worldWidth, int worldHeight)
{
	m_layerName = layerName;
	m_tilesetTexture = texture;
	m_tilesetPath = tilesetKey;
	m_tilesetWidth = texture.width / TILE_SIZE;
	m_tilesetHeight = texture.height / TILE_SIZE;
	
	m_tileLayerData = new Grid<int>(worldWidth, worldHeight);

	for (int xx = 0; xx < m_tileLayerData->GetWidth(); xx++)
	{
		for (int yy = 0; yy < m_tileLayerData->GetHeight(); yy++)
		{
			m_tileLayerData->Set(xx, yy, 0);
		}
	}
}

TileLayer::~TileLayer()
{
	//delete m_tileLayerData;
}

void TileLayer::DrawTilesLayer()
{
	if (!m_visible) return;

	for (int xx = 0; xx < m_tileLayerData->GetWidth(); xx++)
	{
		for (int yy = 0; yy < m_tileLayerData->GetHeight(); yy++)
		{
			if (m_tileLayerData != nullptr)
			{
				DrawTile({ (float)(xx * TILE_SIZE),(float)(yy * TILE_SIZE) }, m_tileLayerData->Get(xx, yy));
				//DrawText(std::to_string(m_tileLayerData->Get(xx, yy)).c_str(), (xx * TILE_SIZE), (float)(yy * TILE_SIZE),12,BLACK);
			}
		}
	}
}

void TileLayer::DrawTile(Vector2 worldPos, int tileIndex)
{
	DrawTexturePro(m_tilesetTexture, GetTileDrawRect(tileIndex), { worldPos.x,worldPos.y,(float)TILE_SIZE,(float)TILE_SIZE }, {0,0},0,WHITE);
}

void TileLayer::DrawTile(Vector2 worldPos, Vector2 tileIndex)
{
	DrawTile(worldPos,Vector2ToTileIndex(tileIndex));
}

int TileLayer::Vector2ToTileIndex(Vector2& gridPosition)
{
	return (((int)(gridPosition.y) * m_tilesetWidth) + (int)(gridPosition.x));
}

Rectangle TileLayer::GetTileDrawRect(int tileIndex)
{
	int xx = tileIndex;
	int yy = 0;

	while (xx >= m_tilesetWidth)
	{
		xx -= m_tilesetWidth;
		yy++;
	}

	return {(float)(xx * TILE_SIZE),(float)(yy * TILE_SIZE),(float)(TILE_SIZE),(float)(TILE_SIZE)};
}

void TileLayer::SetTile(Vector2 gridPosition, int tileIndex)
{
	m_tileLayerData->Set((int)gridPosition.x, (int)gridPosition.y, tileIndex);
}

void TileLayer::SetTile(int gridPosition, int tileIndex)
{
	m_tileLayerData->Set(gridPosition,tileIndex);
}

void TileLayer::SetTile(Vector2 gridPosition, Vector2 tileIndex)
{
	m_tileLayerData->Set((int)gridPosition.x, (int)gridPosition.y, Vector2ToTileIndex(tileIndex));
}

void TileLayer::SetTile(int gridPosition, Vector2 tileIndex)
{
	m_tileLayerData->Set(gridPosition, Vector2ToTileIndex(tileIndex));
}

void TileLayer::FloodFillTiles(int x, int y, int value, int targetValue)
{
	if (!WithinGrid(x, y))
	{
		return;
	}

	if (m_tileLayerData->Get(x, y) == value)
	{
		return;
	}

	if (m_tileLayerData->Get(x, y) != targetValue)
	{
		return;
	}

	m_tileLayerData->Set(x, y, value);

	FloodFillTiles(x - 1, y, value, targetValue);
	FloodFillTiles(x + 1, y, value, targetValue);
	FloodFillTiles(x, y - 1, value, targetValue);
	FloodFillTiles(x, y + 1, value, targetValue);
}

void TileLayer::UpdateAutoTile(Vector2 gridPosition)
{
	int x = (int)gridPosition.x;
	int y = (int)gridPosition.y;

	bool north     = (!WithinGrid(x,y-1))?      true : (Get(x,y-1) > 0);
	bool northEast = (!WithinGrid(x+1, y - 1))? true : (Get(x+1, y - 1) > 0);
	bool northWest = (!WithinGrid(x-1, y-1))?   true : (Get(x + 1, y - 1) > 0);
	bool west      = (!WithinGrid(x-1, y))?     true : (Get(x-1, y) > 0);
	bool east      = (!WithinGrid(x+1, y))?     true : (Get(x+1, y) > 0);
	bool southWest = (!WithinGrid(x-1, y+1))?   true : (Get(x-1, y+1) > 0);
	bool south     = (!WithinGrid(x, y+1))?     true : (Get(x, y+1) > 0);
	bool southEast = (!WithinGrid(x+1, y+1))?   true : (Get(x+1, y+1) > 0);

	int slotValue = (1*northWest) + (2*north) + (4*northEast) + (8*west) +
		        (16 * east) + (32*southWest) + (64*south) + (128 * southEast);


}

void TileLayer::UpdateAutoTile(int pos)
{
}

void TileLayer::SetAutoTile(Vector2 gridPosition)
{
}

void TileLayer::SetAutoTile(int pos)
{
}

int TileLayer::Get(int pos)
{
	return m_tileLayerData->Get(pos);
}

int TileLayer::Get(int x, int y)
{
	return m_tileLayerData->Get(x,y);
}

void TileLayer::SetName(const char* newName)
{
	m_layerName = newName;
}

const char* TileLayer::GetName()
{
	return m_layerName;
}

void TileLayer::Resize(int newWidth, int newHeight)
{
	Grid<int>* newlevelGrid = new Grid<int>(newWidth, newHeight);

	for (int xx = 0; xx < m_tileLayerData->GetWidth() && xx < newWidth; xx++)
	{
		for (int yy = 0; yy < m_tileLayerData->GetHeight() && yy < newHeight; yy++)
		{
			newlevelGrid->Set(xx, yy, m_tileLayerData->Get(xx, yy));
		}
	}

	delete m_tileLayerData;
	m_tileLayerData = newlevelGrid;
}

void TileLayer::GetSolids(Rectangle boundingBox, Vector2 position, std::list<Rectangle>& output)
{
	if (!m_collisionLayer)
	{
		return;
	}

	boundingBox.x = position.x;
	boundingBox.y = position.y;

	//Get min and max possible tiles
	int x1 = floor(boundingBox.x / TILE_SIZE) - 1;
	int x2 = ceil((boundingBox.x + boundingBox.width) / TILE_SIZE) + 1;

	int y1 = floor(boundingBox.y / TILE_SIZE) - 1;
	int y2 = ceil((boundingBox.y + boundingBox.height) / TILE_SIZE) + 1;

	x1 = std::max(x1, 0);
	x2 = std::min(x2, m_tileLayerData->GetWidth());

	y1 = std::max(y1, 0);
	y2 = std::min(y2, m_tileLayerData->GetHeight());

	//Return all solids hitboxes
	for (int xx = x1; xx <= x2; xx++)
	{
		for (int yy = y1; yy <= y2; yy++)
		{
			if (m_tileLayerData->Get(xx, yy) >= 1)
			{
				output.push_back({ (float)(xx * TILE_SIZE),(float)(yy * TILE_SIZE),(float)TILE_SIZE ,(float)TILE_SIZE });
			}
		}
	}
}

void TileLayer::SaveLayer(tinyxml2::XMLDocument& level, tinyxml2::XMLElement* parentElement)
{
	auto tileLayer = level.NewElement("TileLayer");

	tileLayer->SetAttribute("LayerName", m_layerName);
	tileLayer->SetAttribute("TilesetTexture", m_tilesetPath);
	tileLayer->SetAttribute("LayerWidth",m_tileLayerData->GetWidth());
	tileLayer->SetAttribute("LayerHeight", m_tileLayerData->GetHeight());

	tileLayer->SetAttribute("LayerVisible", m_visible);
	tileLayer->SetAttribute("CollisionLayer", m_collisionLayer);

	tinyxml2::XMLElement* tileData = level.NewElement("TileData");

	//Write tile data
	for (int i = 0; i < m_tileLayerData->GetSize(); i++)
	{
		tinyxml2::XMLElement* gridDataElement = level.NewElement("Tile");
		gridDataElement->SetAttribute("TileValue", m_tileLayerData->Get(i));

		tileData->InsertEndChild(gridDataElement);
	}

	tileLayer->InsertEndChild(tileData);
	parentElement->InsertEndChild(tileLayer);
}

int TileLayer::AutoTileToTilesetTile(int autoTileValue)
{
	//Un-godly
	int x = 0;
	int y = 0;

	switch (autoTileValue)
	{
		case 2: {x = 3; y = 1; break; }
		case 8: {x = 0; y = 4; break; }

		case 10: {x = 0; y = 4; break; }
		case 11: {x = 0; y = 4; break; }
	}
	return 0;
}
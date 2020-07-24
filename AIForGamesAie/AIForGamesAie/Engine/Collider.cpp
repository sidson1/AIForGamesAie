#include "Collider.h"
#include "GameObject.h"

Collider::Collider()
{
	
}

void Collider::Setup(GameObject* parent, int width, int height)
{
	m_parent = parent;
	m_boxWidth = width;
	m_boxHeight = height;
}

Rectangle Collider::GetBBox()
{
	if (m_parent == nullptr) return {-4,-4,1,1};

	return {m_parent->GetPosition().x - m_parent->GetOrigin().x, m_parent->GetPosition().y - m_parent->GetOrigin().y,(float)m_boxWidth,(float)m_boxHeight};
}


float Collider::BBoxLeft()
{
	return GetBBox().x;
}

float Collider::BBoxRight()
{
	return GetBBox().x + m_boxWidth;
}

float Collider::BBoxTop()
{
	return GetBBox().y;
}

float Collider::BBoxBottom()
{
	return GetBBox().y + m_boxHeight;
}

float Collider::BBoxWidth()
{
	return m_boxWidth;
}

float Collider::BBoxHeight()
{
	return m_boxHeight;
}

Vector2 Collider::BBoxCenter()
{
	return { GetBBox().x + (m_boxWidth/2),GetBBox().y + (m_boxHeight / 2) };
}

bool Collider::CollideAt(Rectangle rect, const Vector2& position)
{
	Rectangle box = GetBBox();
	box.x = position.x - m_parent->GetOrigin().x;
	box.y = position.y - m_parent->GetOrigin().y;

	return CheckCollisionRecs(rect, box);
}

bool Collider::CollideAt(std::list<Rectangle> rects, const Vector2& position)
{

	if (rects.empty()) return false;

	for (auto const& rect : rects)
	{
		if (CollideAt(rect, position))
		{
			return true;
		}
	}
	return false;
}

bool Collider::CollideAt(Collider coll, const Vector2& position)
{
	return CollideAt(coll.GetBBox(),position);
}

bool Collider::CollideAt(std::list<Collider> colls, const Vector2& position)
{
	if (colls.empty()) return false;

	for (auto const& coll : colls)
	{
		if (CollideAt(coll, position))
		{
			return true;
		}
	}
	return false;
}
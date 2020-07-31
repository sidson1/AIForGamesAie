#include "Graph2D.h"
#include "Graph.h"
#include "raymath.h"
#include <iostream>
#include <functional>

Graph2D::Graph2D()
{

}

Graph2D::Graph2D(std::vector <Graph2D::Node*> nodes)
{
	m_nodes.clear();
	for (auto const& node : nodes)
	{
		m_nodes.push_back(node);
	}
}

Graph2D::~Graph2D()
{

}

std::vector<Graph2D::Node*>  Graph2D::ForEachDijkstra(Graph2D::Node* startNode, Graph2D::Node* endNode, std::function<void(Graph2D::Node * n)> process)
{
	std::list<Graph2D::PathfindNode* > openList;
	std::vector<Graph2D::PathfindNode* > closedList;

	std::vector<Node*> returnNodes;

	openList.push_front(new Graph2D::PathfindNode({ startNode ,nullptr,0 }));

	while (!openList.empty())
	{
		Graph2D::PathfindNode* current = openList.front();
		openList.pop_front();
		closedList.push_back(current);

		//End
		if (current->graphNode == endNode)
		{
			std::cout << current->cost << std::endl;
			while (current->parent != nullptr)
			{
				returnNodes.push_back(current->graphNode);
				current = current->parent;
			}
			returnNodes.push_back(current->graphNode);

			return returnNodes;
		}

		//Edges
		for (auto const& edge : current->graphNode->connections)
		{
			bool doAdd = true;

			//Check Q
			for (auto const& opened : openList)
			{
				if (opened ->graphNode == edge.to)
				{
					doAdd = false;
					break;
				}
			}

			//Check closed
			for (auto const& closed : closedList)
			{
				if (closed->graphNode == edge.to)
				{
					if (closed->cost > current->cost + edge.data)
					{
						closed->cost = current->cost + edge.data;
						closed->parent = current;
					}
					doAdd = false;
					break;
				}
			}

			if (doAdd)
			{
				openList.push_front(new Graph2D::PathfindNode({ edge.to ,current,current->cost + edge.data }));
			}
		}


		openList.sort([](const Graph2D::PathfindNode* a, const Graph2D::PathfindNode* b) {
			return a->cost < b->cost;
		});
	
	}

	return returnNodes;
}

std::vector<Graph2D::Node*> Graph2D::GetNearbyNodes(Vector2 position, float radius)
{
	std::vector<Graph2D::Node*> returnList;

	for (auto node : m_nodes)
	{
		float dist = Vector2Distance(position, node->data);

		if (dist <= radius)
		{
			returnList.push_back(node);
		}
	}
	return returnList;
}


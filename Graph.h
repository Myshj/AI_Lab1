#ifndef GRAPH_H
#define GRAPH_H

#include "vector"

#include "set"

#include <unordered_map>

#include "SetColorErrors.h"
#include "GetColorErrors.h"
#include "GetCountOfConflictsInVertexErrors.h"
#include "getadjacentvertexeserrors.h"
#include "getworstbetweenadjacenterrors.h"

#include "AddVertexErrors.h"
#include "AddConnectionErrors.h"
#include "AddColorErrors.h"

#include "findbestalternativecolorerrors.h"




/*Represents oriented graph.*/
class Graph
{
    public:
        Graph();
        ~Graph();

        /*
        Sets new color for specified vertex.
        */
        SetColorErrors SetColor(const unsigned int vertexId, const unsigned int newColor)
        {
            if(!IsValidVertexId(vertexId))
            {
                return SetColorErrors::IncorrectVertex;
            }

            if(!IsValidColorId(newColor))
            {
                return SetColorErrors::IncorrectColor;
            }

            vertexColors[vertexId] = newColor;
        }

        /*
        Returns current color of specified vertex in parameter outColor.
        Returns 0 if specified vertex does not exist.
        */
        GetColorErrors GetColor(const unsigned int vertexId, unsigned int& outColor) const
        {
            if(!IsValidVertexId(vertexId))
            {
                outColor = 0;

                return GetColorErrors::IncorrectVertex;
            }

            outColor =  vertexColors.at(vertexId);

            return GetColorErrors::None;
        }

        /*
        Returns count of vertexes with colorId that are adjacent to vertexId in parameter outCountOfConflicts.
        Returns 0 if specified vertex does not exist or specified color does not exist.
        */
        GetCountOfConflictsInVertexErrors GetCountOfConflictsInVertex(const unsigned int vertexId,
                                                                      const unsigned int colorId,
                                                                      unsigned int& outCountOfConflicts) const
        {
            outCountOfConflicts = 0;

            if(!IsValidVertexId(vertexId))
            {
                return GetCountOfConflictsInVertexErrors::IncorrectVertex;
            }

            if(!IsValidColorId(colorId))
            {
                return GetCountOfConflictsInVertexErrors::IncorrectColor;
            }

            for(auto adjacentVertexId: adjacencies.at(vertexId))
            {
                unsigned int colorOfAdjacentVertex = 0;

                GetColor(adjacentVertexId, colorOfAdjacentVertex);

                if(colorOfAdjacentVertex == colorId)
                {
                    outCountOfConflicts += 1;
                }
            }

            return GetCountOfConflictsInVertexErrors::None;
        }

        unsigned int GetOverallCountOfConflicts() const
        {
            unsigned int ret = 0;

            for(auto vertexId : possibleVertexIds)
            {
                unsigned int colorOfCurrentVertex = 0;
                GetColor(vertexId, colorOfCurrentVertex);

                unsigned int countOfConflictsInCurrentVertex = 0;
                GetCountOfConflictsInVertex(vertexId,
                                            colorOfCurrentVertex,
                                            countOfConflictsInCurrentVertex);

                ret += countOfConflictsInCurrentVertex;
            }

            return ret;
        }

        unsigned int GetWorstCountOfConflicts() const
        {
            unsigned int ret = 0;

            for(auto vertexId : possibleVertexIds)
            {
                unsigned int colorOfCurrentVertex = 0;
                GetColor(vertexId, colorOfCurrentVertex);

                unsigned int countOfConflictsInCurrentVertex = 0;
                GetCountOfConflictsInVertex(vertexId,
                                            colorOfCurrentVertex,
                                            countOfConflictsInCurrentVertex);

                if(countOfConflictsInCurrentVertex > ret)
                {
                    ret = countOfConflictsInCurrentVertex;
                }
            }

            return ret;
        }

        GetWorstBetweenAdjacentErrors GetWorstBetweenAdjacent(const unsigned int vertexId,
                                unsigned int& outWorstBetweenAdjacentId) const
        {
            outWorstBetweenAdjacentId = 0;

            if(!IsValidVertexId(vertexId))
            {
                return GetWorstBetweenAdjacentErrors::IncorrectVertex;
            }

            std::set<unsigned int> adjacentVertexes;
            GetAdjacentVertexes(vertexId, adjacentVertexes);

            if(adjacentVertexes.size() == 0)
            {
                return GetWorstBetweenAdjacentErrors::VertexIsIsolated;
            }

            for(auto adjacentVertex : adjacentVertexes)
            {
                unsigned int colorOfCurrentVertex = 0;
                GetColor(adjacentVertex, colorOfCurrentVertex);

                unsigned int countOfConflictsInCurrentVertex = 0;
                GetCountOfConflictsInVertex(adjacentVertex,
                                            colorOfCurrentVertex,
                                            countOfConflictsInCurrentVertex);

                if(countOfConflictsInCurrentVertex >= outWorstBetweenAdjacentId)
                {
                    outWorstBetweenAdjacentId = countOfConflictsInCurrentVertex;
                }
            }

            return GetWorstBetweenAdjacentErrors::None;
        }

        FindBestAlternativeColorErrors FindBestAlternativeColor(const unsigned int vertexId,
                                                                unsigned int& outNewColorId) const
        {
            outNewColorId = 0;

            if(!IsValidVertexId(vertexId))
            {
                return FindBestAlternativeColorErrors::IncorrectVertex;
            }

            unsigned int currentColor = 0;
            GetColor(vertexId, currentColor);

            unsigned int currentBestCountOfConflicts = 0;


            for(auto colorId : possibleColorIds)
            {
                if(colorId == currentColor)
                {
                    continue;
                }

                unsigned int countOfConflictsWithNewColor = 0;
                GetCountOfConflictsInVertex(vertexId, colorId, countOfConflictsWithNewColor);

                if(countOfConflictsWithNewColor >= 0)
                {
                    currentBestCountOfConflicts = countOfConflictsWithNewColor;

                    for(auto colorId : possibleColorIds)
                    {
                        if(colorId == currentColor)
                        {
                            continue;
                        }

                        GetCountOfConflictsInVertex(vertexId, colorId, countOfConflictsWithNewColor);

                        if(countOfConflictsWithNewColor <= currentBestCountOfConflicts)
                        {
                            currentBestCountOfConflicts = countOfConflictsWithNewColor;

                            outNewColorId = colorId;
                        }
                    }

                    break;
                }
            }

            return FindBestAlternativeColorErrors::None;
        }

        /*
        Adds new vertex to graph.
        If vertex with provided identifier does already exist adds nothing.
        */
        AddVertexErrors AddVertex(const unsigned int vertexId)
        {
            if(IsValidVertexId(vertexId))
            {
                return AddVertexErrors::VertexAlreadyExists;
            }

            possibleVertexIds.insert(vertexId);

            adjacencies[vertexId] = std::set<unsigned int>();

            return AddVertexErrors::None;
        }

        /*
        Adds connection between two specified vertexes.
        If at least one of provided ids is incorrect or connection does already exist adds nothing.
        */
        AddConnectionErrors AddConnection(const unsigned int vertexFromId,
                                          const unsigned int vertexToId)
        {
            if(!IsValidVertexId(vertexFromId))
            {
                return AddConnectionErrors::IncorrectVertexFrom;
            }

            if(!IsValidVertexId(vertexToId))
            {
                return AddConnectionErrors::IncorrectVertexTo;
            }

            if(DoesConnectionExist(vertexFromId, vertexToId))
            {
                return AddConnectionErrors::ConnectionAlreadyExists;
            }

            adjacencies[vertexFromId].insert(vertexToId);

            return AddConnectionErrors::None;
        }

        /*
        Add new possible color to graph.
        If color with specified id does already exist adds nothing.
        */
        AddColorErrors AddColor(const unsigned int colorId)
        {
            if(IsValidColorId(colorId))
            {
                return AddColorErrors::ColorAlreadyExists;
            }

            possibleColorIds.insert(colorId);

            return AddColorErrors::ColorAlreadyExists;
        }

        std::set<unsigned int> GetPossibleColorIds() const
        {
            return possibleColorIds;
        }

        std::set<unsigned int> GetPossibleVertexIds() const
        {
            return possibleVertexIds;
        }

        unsigned int GetCountOfVertexes() const
        {
            return adjacencies.size();
        }

        GetAdjacentVertexesErrors GetAdjacentVertexes(const unsigned int vertexId,
                                                      std::set<unsigned int>& outAdjacentVertexes) const
        {
            outAdjacentVertexes = std::set<unsigned int>();

            if(!IsValidVertexId(vertexId))
            {
                return GetAdjacentVertexesErrors::IncorrectVertex;
            }

            outAdjacentVertexes = adjacencies.at(vertexId);

            return GetAdjacentVertexesErrors::None;
        }

        /*
        Returns true if a valid vertex identifier provided.
        */
        bool IsValidVertexId(const unsigned int vertexId) const
        {
            return possibleVertexIds.find(vertexId) != possibleVertexIds.end();
        }

        /*
        Returns true if a valid color identifier provided.
        */
        bool IsValidColorId(const unsigned int colorId) const
        {
            return possibleColorIds.find(colorId) != possibleColorIds.end();
        }

        /*
        Returns true if connection between two vertexes exists.
        Returns false if one of provided ids is incorrect or there are no connection.
        */
        bool DoesConnectionExist(const unsigned int vertexFromId,
                                 const unsigned int vertexToId) const
        {
            if(!(IsValidVertexId(vertexFromId) && IsValidVertexId(vertexToId)))
            {
                return false;
            }

            auto& verticiesAdjacentToVertexFrom = adjacencies.at(vertexFromId);

            return verticiesAdjacentToVertexFrom.find(vertexToId) != verticiesAdjacentToVertexFrom.end();
        }

        bool IsColoredProperly() const
        {
            for(auto vertexId : possibleVertexIds)
            {
                unsigned int colorOfVertex = 0;
                GetColor(vertexId, colorOfVertex);

                unsigned int countOfConflictsInVertex = 0;
                GetCountOfConflictsInVertex(vertexId, colorOfVertex, countOfConflictsInVertex);

                if(countOfConflictsInVertex > 0)
                {
                    return false;
                }
            }

            return true;
        }

    protected:

    private:

        /*
        Table of adjacency.
        Key: vertex identifier.
        Value: set of vertex identifiers that are adjacent to.
        */
        std::unordered_map<unsigned int, std::set<unsigned int> > adjacencies;

        /*
        Key: vertex identifier,
        Value: color identifier.
        */
        std::unordered_map<unsigned int, unsigned int> vertexColors;

        /*Contains unique vertex identifiers.*/
        std::set<unsigned int> possibleVertexIds;

        /*Contains unique color identifiers.*/
        std::set<unsigned int> possibleColorIds;
};

#endif // GRAPH_H

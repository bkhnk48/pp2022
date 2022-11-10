//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef VEINS_INET_ADAPTIVESYSTEM_H_
#define VEINS_INET_ADAPTIVESYSTEM_H_

/*
 * AcoPath: Shortest path calculation using Ant Colony Optimization
 * Copyright (C) 2014-2021 by Constantine Kyriakopoulos
 * zfox@users.sourceforge.net
 * @version 0.9.1
 *
 * @section LICENSE
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Constant.h"
//#include <omp.h>

using boost::property_tree::ptree;

enum STATE_OF_REQUEST{
    WAITING_FOR_PROCESSING,
    BEING_PROCESSED,
    FINISHED
};

typedef std::tuple<int, int, std::string, double, STATE_OF_REQUEST, std::string> Request;

class AdaptiveSystem
{
public:
    struct Edge
    {
        Edge();
        int edgeStart;
        int edgeEnd;
        double weight;
        long int id;
        std::string src;
        std::string dst;
        bool isBVertex;
        bool operator<(const Edge&) const;
        bool operator>(const Edge&) const;
        bool operator==(const Edge&) const;
    };

    AdaptiveSystem();
    virtual ~AdaptiveSystem();
    virtual std::vector<int> path(int, int) = 0;
    virtual void insertEdge(int, int, double) noexcept(false);
    virtual void clear() = 0;
    bool isWorking();//OpenMP threads are running or not
    bool isFullReqs(); //checking the number of reqs exceeds the maximum value or not
    virtual bool insertRequest(int source, int dst, std::string id);
    virtual bool removeExpiredRequests(std::vector<int>* expiredRequests);
    virtual bool canExecuteReqs();
    virtual bool hasIdenticalReq();
    std::vector<Request> allRequests;

protected:
    virtual void initTopo(const std::string&);
    std::vector<Edge> adaptiveEdges;

private:
    static int edgeIdCnt;
};

#endif /* VEINS_INET_ADAPTIVESYSTEM_H_ */

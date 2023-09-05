#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <regex>
#include "Point.cpp"
#include "Shape.cpp"
#include "TimeExpandedNode.cpp"
#include <assert.h>

using namespace std;

// Hàm getCoincedence như sau
vector<pair<int, int>> getConcidences(vector<pair<TimeExpandedNode *, Shape *>> *srcs,
                                      vector<pair<TimeExpandedNode *, Shape *>> *tgts)
{
    vector<pair<int, int>> result;
    for (int i = 0; i < srcs->size(); i++)
    {
        for (int j = 0; j < tgts->size(); j++)
        {
            if (srcs->at(i).second->equals(tgts->at(j).second))
            {
                result.push_back(make_pair(i, j));
            }
        }
    }
    return result;
}

// Hàm dùng Regular Expression để bắt chuỗi
string strSmatch(string str, string W_regex)
{
    regex word_regex(W_regex);
    smatch match;
    regex_search(str, match, word_regex);
    return match.str();
}

// Hàm kiểm tra xem điểm point có trong vector P hay không
bool checkPoint(Point *point, vector<Point *> P)
{
    if (P.empty())
        return false;
    for (Point *p : P)
    {
        if (point->equals(p))
            return true;
    }
    return false;
}

// Hàm kiểm tra xem shape có trong vector S hay không
bool checkShape(Shape *shape, vector<Shape *> S)
{
    if (S.empty())
        return false;
    for (Shape *s : S)
    {
        if (shape->equals(s))
            return true;
    }
    return false;
}

int main()
{
    vector<Point *> P;
    vector<Shape *> S;
    vector<vector<TimeExpandedNode *>> allTENs;
    vector<TimeExpandedNode *> tempTENs;
    allTENs.push_back(tempTENs);

    ifstream input("AllParts.txt");
    input.is_open();
    int counter = 0;

    while (!input.eof())
    {
    	/*counter++;
    	if(counter == 4065){
    	    cout<<"DEBUG: "<<__FILE__<<":"<<__LINE__<<endl;
    	}*/
        string str;
        getline(input, str);
        if (strSmatch(str, "[_A-Z0-9]+(?= )") == "")
            break;
        Shape *shape = new Shape();

        shape->setName(strSmatch(str, "[_A-Z0-9]+(?= )"));

        string point_1 = strSmatch(str, "[_][-0-9.,]+(?=_)");
        string point_2 = strSmatch(str, "[^_A-Z ][-0-9.,]+(?=$)");

        string s1 = "[^_ ][-0-9.,]+(?=,)";
        string s2 = "[^_,][-0-9.]+(?=$)";
        string s3 = "[^_,][-0-9.]+(?=$)";

        Point *firstPoint = new Point();
        firstPoint->setPoint(std::stof(strSmatch(point_1, s1)),
                             std::stof(strSmatch(point_1, s2)));
        Point *lastPoint = new Point();
        lastPoint->setPoint(std::stof(strSmatch(point_2, s1)),
                            std::stof(strSmatch(point_2, s3)));
	if(firstPoint->equals(lastPoint)){
	    //cout<<"DEBUG "<<__FILE__<<":"<<__LINE__<<endl;
	    continue;
	}
        shape->setPoint(firstPoint, lastPoint);

        if (!checkPoint(firstPoint, P))
        {
            P.push_back(firstPoint);
            TimeExpandedNode *n1 = new TimeExpandedNode();
            n1->setTENode(firstPoint);
            allTENs.at(0).push_back(n1);
        }

        if (!checkPoint(lastPoint, P))
        {
            P.push_back(lastPoint);
            TimeExpandedNode *n2 = new TimeExpandedNode();
            n2->setTENode(lastPoint);
            allTENs.at(0).push_back(n2);
        }

        if (!checkShape(shape, S))
        {
            S.push_back(shape);
        }
    }

    for (TimeExpandedNode *n : allTENs.at(0))
    {
        for (Shape *s : S)
        {
            if (s->start->equals(n->origin))
            {
                n->insertTarget(s);
            }
            else if (s->end->equals(n->origin))
            {
                n->insertSource(s);
            }
        }
    }
    //cout<<"End of first nested-for loops"<<endl;
    //cout<<"allTENs.at(0).size( ) = "<<allTENs.at(0).size()<<endl;

    for (TimeExpandedNode *nA : allTENs.at(0))
    {
        for (TimeExpandedNode *nB : allTENs.at(0))
        {
            vector<pair<int, int>> nA_to_nB;
            vector<pair<int, int>> nB_to_nA;
            if (!nA->equals(nB))
            {
                nB_to_nA = getConcidences(&(nA->srcs), &(nB->tgts));
                nA_to_nB = getConcidences(&(nB->srcs), &(nA->tgts));
                nA->insertSourcesAndTargets(nB, nB_to_nA, nA_to_nB);
            }
        }
    }
    //cout<<"End of second nested-for loop"<<endl;
  

    for (Point *p : P)
    {
        int i = 0;
        for (TimeExpandedNode *n : allTENs.at(0))
        {
            if (p->equals(n->origin))
                i++;
            assert(n->time == 0);
            // Tất cả các trường time của tất cả các TENode đều bằng 0
        }
        assert(i = 1);
        // mọi point chỉ xuất hiện một lần trong tất cả các trường origin của tất cả các TENode
    }
    //cout<<"End of third nested-for loop"<<endl;

    for (Shape *s : S)
    {
        int i = 0;
        for (TimeExpandedNode *n : allTENs.at(0))
        {
            for (pair<TimeExpandedNode *, Shape *> x : n->srcs)
            {
                if (s->equals(x.second))
                    i++;

                assert(x.first != nullptr && x.second != nullptr);
                // Trong các vector srcs và tgts của các TENode, nếu vector nào khác rỗng thì tất cả các trường của tất
                // cả các phần tử trong vector đều khác null.
            }

            for (pair<TimeExpandedNode *, Shape *> x : n->tgts)
            {
                if (s->equals(x.second))
                    i++;
		if(x.first == nullptr || x.second == nullptr){
			cout<<"DEBUYG"<<endl;
		}
                assert(x.first != nullptr && x.second != nullptr);
                // Trong các vector srcs và tgts của các TENode, nếu vector nào khác rỗng thì tất cả các trường của tất
                // cả các phần tử trong vector đều khác null.
            }
        }
        assert(i = 2);
        // Mọi shape chỉ xuất hiện hai lần trong tất cả các trường srcs và tgts của tất cả các TENode.
    }
    //cout<<"End of forth nested-for loop"<<endl;

    for (TimeExpandedNode *n : allTENs.at(0))
    {
        int sum = n->srcs.size() + n->tgts.size();
        int i = 0;
        for (TimeExpandedNode *m : allTENs.at(0))
        {
            for (pair<TimeExpandedNode *, Shape *> x : m->srcs)
            {
                if (n == x.first)
                    i++;
            }

            for (pair<TimeExpandedNode *, Shape *> x : m->tgts)
            {
                if (n == x.first)
                    i++;
            }
        }
        assert(sum == i);
        // Nếu phần tử n có tổng độ dài của srcs và tgts của nó là X. Thì số lần xuất hiện của n trong các vector
        // srcs và tgts của các TENodes khác cũng sẽ bằng đúng X
    }
    //cout<<"End of fifth nested-for loop"<<endl;
    cout<<"DONE."<<endl;
}

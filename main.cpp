//ifrah saleem 26238
#include <iostream> 
#include <string>
#include <fstream>
#include <sstream>
#include <cmath> 
#include <vector>
using namespace std; 
  
// Used to hold details of a coordinate
struct Point 
{ 
    int x; 
    int y; 
    Point(int xx, int yy) 
    { 
        x = xx; 
        y = yy; 
    } 
    Point() 
    { 
        x = 0; 
        y = 0; 
    } 
}; 
  
class Node
{
private: 
	Point pos; 
    string city; 

	// Children of this tree 
    Node *NW; 
    Node *NE; 
    Node *SE; 
    Node *SW; 

public:
	Node():city(""),NW(NULL),NE(NULL),SE(NULL),SW(NULL){};
	Node(string name, Point p, Node* nw, Node * ne, Node * se, Node * sw):city(name),pos(p),NW(nw),NE(ne),SE(se),SW(sw){};
	friend class Quad;
};
  
// The main quadtree class 
class Quad 
{ 

private: 
    // Hold details of the boundary of this node 
    Point upperRight; 
    Point lowerLeft; 
  
    // Contains details of node 
	Node *r;
	void insert(Node* node, Node* root);
	void print(Node * node);
	void search(int x, int y, int radius, Node * root, vector<string> & vec, bool & found);
  
public: 
    Quad();
    Quad(Point topR, Point lowL);
    void insert(Node* node); 
    void search(int x, int y, int radius, vector<string> & vec, bool & found); 
    bool inBoundary(Point); 
	void pretty_print();
}; 

//constructors
Quad::Quad()
{ 
    lowerLeft = Point(0, 0); 
    upperRight = Point(0, 0); 
	r = NULL;
} 

Quad::Quad(Point topR, Point lowL)
{ 
    lowerLeft = lowL; 
    upperRight = topR; 
	r = NULL;
} 

void Quad::insert(Node *node)
{
	insert(node, r);
}

// Insert a node into the quadtree 
void Quad::insert(Node *node, Node* root) 
{ 
    if (node == NULL) 
        return; 
  
    // Current quad cannot contain it 
    if (!inBoundary(node->pos)) 
        return; 
  
    // We are at a quad of unit area 
    // We cannot subdivide this quad further 
    if (abs(upperRight.x - lowerLeft.x) <= 1 && 
        abs(upperRight.y - lowerLeft.y) <= 1) 
    { 
        if (root == NULL) 
            root = node; 
        return; 
    } 

	if (r == NULL)
	{
		r = node;
		return;
	}

	if (root == NULL) 
	{
		root = node; 
		return;
	}

	//else if(node->pos.x < root->pos.x)//either southwest or northwest
	//{
		if(node->pos.x < root->pos.x && node->pos.y < root->pos.y) //southwest
		{
			if (root->SW == NULL) 
                root->SW = node;

			else
				insert(node, root->SW); 

		}

		else if(node->pos.x < root->pos.x && node->pos.y >= root->pos.y)//northwest
		{
			 if (root->NW == NULL) 
                root->NW = node;

			 else
				insert(node, root->NW);

		}
	//}

	//else //either southeast or northeast

	//{
		else if(node->pos.x >= root->pos.x && node->pos.y < root->pos.y) //southeast
		{
			if (root->SE == NULL) 
                root->SE = node;

			 else
				insert(node, root->SE);
		}

		else if(node->pos.x >= root->pos.x && node->pos.y >= root->pos.y) //northeast
		{
			if (root->NE == NULL) 
                root->NE = node;

			 else
				insert(node, root->NE);
		}
	//}
} 
  
 //Find a node in a quadtree 
void Quad::search(int x, int y, int radius, Node * root, vector<string> & vec, bool & found) 
{ 
    // Current quad cannot contain it 
    if( x >= upperRight.x && 
		x <= lowerLeft.x && 
		y >= upperRight.y && 
		y <= lowerLeft.y)
		cout << "<None>" << endl; 
  
    // We are at a quad of unit length 
    // We cannot subdivide this quad further 
    if (r == NULL) 
        cout << "<None>" << endl;
  
    // We are at a quad of unit area 
    // We cannot subdivide this quad further 
    if (abs(upperRight.x - lowerLeft.x) <= 1 && 
        abs(upperRight.y - lowerLeft.y) <= 1) 
    { 
        if (r == NULL) 
			cout << "<None>" << endl; 
    } 

	if (r == NULL)
		cout << "<None>" << endl;
		
	else if (root == NULL) 
	{
		//cout << "<None>" << endl;
		return;
	}
	
	//calculate distance from root 
	double distance = sqrt(pow((x - root->pos.x),2) + pow((y - root->pos.y),2));
	vec.push_back(root -> city);

	//check if root exists within the radius
	if(distance <= radius) //region 13
	{
		if(!found)
			cout << root -> city ;
		
		else
			cout << ", " << root -> city;
		
		found = true;

		search(x,y,radius,root->SE,vec, found);
		search(x,y,radius,root->SW,vec, found);
		search(x,y,radius,root->NE,vec, found);
		search(x,y,radius,root->NW,vec, found);
	}

	else if(x - radius > root->pos.x && y + radius < root->pos.y) //region 1
	{
		search(x,y,radius,root->SE,vec, found);
	}

	else if(x - radius < root->pos.x && x + radius > root->pos.x && y + radius < root->pos.y)//region 2
	{
		search(x,y,radius,root->SE,vec, found);
		search(x,y,radius,root->SW,vec, found);
	}
	else if(x + radius <= root->pos.x && y + radius < root->pos.y)//region 3
	{
		search(x,y,radius,root->SW,vec, found);
	}
	else if(x + radius < root->pos.x && y + radius > root->pos.y && y - radius < root->pos.y) //region 5
	{
		search(x,y,radius,root->SW,vec, found);
		search(x,y,radius,root->NW,vec, found);
	}

	else if(x + radius < root->pos.x && y - radius > root->pos.y) //region 8
	{
		search(x,y,radius,root->NW,vec, found);
	}
	else if(x - radius < root->pos.x && x + radius > root->pos.x && y - radius > root->pos.y)//region 7
	{
		search(x,y,radius,root->NE,vec, found);
		search(x,y,radius,root->NW,vec, found);
	}
	else if(x - radius > root->pos.x && y - radius > root->pos.y) //region 6
	{
		search(x,y,radius,root->NE,vec, found);
	}
	else if(x - radius > root->pos.x && y + radius > root->pos.y && y - radius < root->pos.y) //region 4
	{
		search(x,y,radius,root->SE,vec, found);
		search(x,y,radius,root->NE,vec, found);
	}
	else if(root -> pos.x > x -radius && root->pos.x < x && y + radius > root->pos.y && y < root->pos.y) //region 9
	{
		search(x,y,radius,root->SE,vec, found);
		search(x,y,radius,root->SW,vec, found);
		search(x,y,radius,root->NE,vec, found);
	}
	else if(root -> pos.x < x + radius && root->pos.x > x && y + radius > root->pos.y && y < root->pos.y) //region 10
	{
		search(x,y,radius,root->SE,vec, found);
		search(x,y,radius,root->SW,vec, found);
		search(x,y,radius,root->NW,vec, found);
	}
	else if(root -> pos.x > x - radius && root->pos.x < x && y - radius < root->pos.y && y > root->pos.y) //region 11
	{
		search(x,y,radius,root->SE,vec, found);
		search(x,y,radius,root->NE,vec, found);
		search(x,y,radius,root->NW,vec, found);
	}
	else //if(x + radius > root -> pos.x && root -> pos.x > x && y - radius < root->pos.y && y > root->pos.y) //region 12
	{
		search(x,y,radius,root->SW,vec, found);
		search(x,y,radius,root->NE,vec, found);
		search(x,y,radius,root->NW,vec, found);

	}
}; 
  
void Quad::search(int x, int y, int radius, vector<string> & vec, bool & found)
{
	search(x, y, radius, r, vec, found);
}

// Check if current quadtree contains the point 
bool Quad::inBoundary(Point p) 
{ 
	if(p.x >= upperRight.x && 
		p.x <= lowerLeft.x && 
		p.y >= upperRight.y && 
		p.y <= lowerLeft.y)
		return false; 
} 

void Quad::pretty_print()
{
	if (r == NULL)
	{
		cout << "Tree is empty" << endl;
	}

	else 
		print(r);
}

void Quad::print(Node * root) 
{
		if(root != NULL) // if the tree is not empty
		{
			cout << root -> city << endl;
			print(root -> SE); // recursively print the south east subtree
			print(root -> SW); // recursively print the south west subtree
			print(root -> NE); // recursively print the north east subtree
			print(root -> NW); // recursively print the north west subtree
		}
}
  
// Driver program 
int main() 
{ 
	string cityFile = "cities.txt", queryFile= "queries.txt" , upperX, upperY, line, cityName, x, y;
	ifstream input1, input2;
	string radius;
	vector<string> vec;
	bool found = false;

	input1.open(cityFile.c_str()); // opening file 
	input2.open(queryFile.c_str()); // opening file 

	if (input1.fail() || input2.fail()) // if file fails to open
		cout << endl << "Cannot find the file(s)" << endl << endl;

	else 
	{
		getline(input1, line);

		istringstream ss(line);

		ss >> upperX >> upperY;

		int intX = stoi(upperX);
		int intY = stoi(upperY);

		if(intX < 0 || intY < 0)
		{
			cout << "Coordinate cannot be a negative value." << endl;
			return 0;
		}

		Quad center(Point(intX, intY), Point(0, 0)); 

		while(getline(input1,line))
		{
			istringstream ss(line);

			ss >> cityName >> x >> y;

			int intx = stoi(x);
			int inty = stoi(y);

			if(intx < 0 || inty < 0)
			{
				cout << "Coordinate cannot be a negative value." << endl;
				return 0;
			}

			else 
			{
				if(intX < intx || intY < inty)
					;
				else
				{
					Node * city = new Node (cityName, Point(intx,inty), NULL, NULL, NULL, NULL);
					center.insert(city);
				}
			}
		}

		center.pretty_print();
		cout << endl;

		while(getline(input2,line))
		{
			istringstream ss(line);

			ss >> x >> y >> radius;

			int intx = stoi(x);
			int inty = stoi(y);
			int intradius= stoi(radius);

			if(intx < 0 || inty < 0 || intradius < 0)
			{
				cout << "Coordinates/radius cannot be a negative value." << endl;
				return 0;
			}

			if(intx > intX || inty > intY)
			{
				cout << "Coordinates lie beyond the boundary." << endl;
				return 0;
			}

			else 
			{
				center.search(intx, inty, intradius, vec, found);
			}

			if(vec.size() == 0 || !found)
				cout << "<None>";

			cout << endl;
			for(unsigned int i = 0; i < vec.size() - 1; i++)
				cout << vec[i] << ", ";
			cout << vec[vec.size()-1];
			vec.clear();
			cout <<endl<<endl;
			found = false;
		}
	}
    return 0; 
} 
#ifndef POLYGON_
#define POLYGON_

#include "glm/glm/vec2.hpp"
#include <vector>

class Polygon{

public:

	// vertices of the polygon
	std::vector<glm::vec2> vertices;
	
	Polygon(){
	
	}
	
	// Add vertices to this->vertices.
	void addVertex(glm::vec2 vp){
		vertices.push_back(vp);
	}
	
	// Draws the polygon
	void draw(){
		
		for(unsigned int i=0; i<vertices.size(); i++)
		{
			glBegin(GL_LINES);

				// Polygon lines will be drawn red.
				glColor3f(1.0f,0.0f,0.0f);

				/** 
				 * Draws the polygon edges by connecting two consecutive vertices.
				 * In order to close the polygon, the last vertex connects to the first one. 
				 */
				
				glVertex2f(vertices[i].x,vertices[i].y); glVertex2f(vertices[(i+1)%vertices.size()].x,vertices[(i+1)%vertices.size()].y);

		    glEnd();
		}
	}
private:

};


#endif

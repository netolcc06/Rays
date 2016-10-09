#ifndef LINE_
#define LINE_

#include <vector>
#include <algorithm>
#include <cmath>
#include "glm/glm/vec2.hpp"
#include "glm/glm/geometric.hpp"
#include "Polygon.hpp"

template <typename T>
class Line{

public:

	/**
	 * The line is specified in the ay + bx + c = 0 form.
	 * Since we have a line, we can write it as a first degree polynomial y = f(x) = (-bx -c)/a
	 */
	Line(T a_, T b_, T c_ ): a(a_), b(b_), c(c_){

	}
	
	/**
	 * Returns the value of the function at x.
	 * ay+bx+C = 0 => y = (-bx -c)/a
	 */
	T at(float x){return (-b*x-c)/a;}

	/** 
	 * This is the vector that contains all the x intercepts of the line with screen space and polygon.
	 * Since the line is a function, having the x coordinate we can easily find the y intercepts of the
	 * line by using the function this->at(x).
     */
	std::vector<T> intercepts;

	/**
	 * Here we draw the lines that lie inside the screen space.
	 * Basically, if a line is inside the screen space, it contains at least 2 intersection points.
	 * If the line intersects the polygon, it will have more than 2 intersection points.
	 * Since the first and the last points intersect walls of the screen/window space, all the other points
	 * come from intersections with the given polygon.
	 * Then, as example, taking a line that has 4 intersection points (p0, p1, p2, p3), p0p1 is outside the polygon,
	 * p1p2 is inside the polygon, and p2p3 is outside the polygon - p0 and p3 are the intersections with the
	 * screen/window space.
	 * My solution has one flaw. When a line intersects a polygon right at one of its corners without getting inside
	 * of the polygon itself, it will change the color line anyway. I didn't get a simple solution for this for
	 * myself. If the polygon was restricted to be convex, this issue would be simplified.
	 * This code was entirely developed by me without any consult to external sources. I took very seriously
	 * the "Most importantly, it should be your own" request. Maybe there is an easy solution to this flaw,
	 * but I couldn't figure out a one that worked for all the cases I have in mind - especially the case in which
     * I have a line that intersects a concave polygon in multiple vertices never getting inside of the polygon.
	 * The first idea that I had  to tackle the problem was related to transform the ay+bx+c=0 line into a ray,
	 * taking something as r(t)=(1, -b/a)t + (0, -c/a), but then I realized that I could solve it analytically
	 * in a straightforward way.
     */
	void draw(){
		
		// If there are less than two intersections, the line is not inside the screen space.
		if(intercepts.size() < 2)
			return;

		// We need to sort x intercepts so we can properly draw the line segments
		std::sort(intercepts.begin(), intercepts.begin()+intercepts.size());

		for(unsigned int i=0; i<intercepts.size()-1; i++)
		{
			glBegin(GL_LINES);

				/**
				 * The first line segment - expected to be outside the polygon - is drawn gray.
				 * If you have another line segment, it will be inside the polygon and will be drawn green.
				 * The next one will be outside the polygon, will be drawn gray and so on.
				 */
				if(i%2 == 0)
					glColor3f(0.55f,0.55f,0.55f); //gray color
				else
					glColor3f(0.0f,1.0f,0.0f); // green color

				/**
				 * The line segment is drawn by taking two consecutive intersections.
				 * intercepts[i] holds the x intercept of the function and we obtain the y intercept by using
				 * this->at(intercepts[i]).
				 */
				glVertex2f(intercepts[i],this->at(intercepts[i]));
				glVertex2f(intercepts[i+1],this->at(intercepts[i+1]));  

				

		    glEnd();
		}
	}

	 /** 
	  * Function used to calculate te intersection between our line and the line segment determined by the points
	  * v1 and v2.*/ 
	void intersect(glm::vec2 v1, glm::vec2 v2){
		/**
		 * The first thing we do here is transforming the line segment specified by the points v1 and v2
		 * into a function/line equation y = mx + b.
		 * m is the angular coefficient of the line and b is the x intercept for y = 0.
		 */
		T m, b, t, upboundx = v1.x, downboundx = v2.x, upboundy = v1.y, downboundy = v2.y;
		// A threshold to be used when calculating the intercept points.
		double threshold = 0.0005;
		
		/** 
		 * If the x's coordinates of v1 and v2 are the same, we have no function to be defined and our line
		 * intersects the line segment when x == v1.x == v2.x.
		 * This is the basic case when this line segment is parallel to the y axis.
		 */
		if(v1.x == v2.x)
			t = v1.x;
		else{
			// Angular coefficient = tangent of the angle theta that the line segment does with the x axis.			
			m = (v2.y-v1.y)/(v2.x-v1.x);
		
			// (I) y1 = m*x1 + b
			// (II) y2 = m*x2 + b
			// (III) (y1+y2) = m*(x1+x2) + 2*b
			// (IV) b = ((y1+y2)-m*(x1+x2))/2
			b = (v1.y + v2.y - m*(v1.x+v2.x))/2.0;

			//We have parallel lines and they never reach each other.
			if((-this->b/this->a) == m) return;

			/** 
			 * After we have two lines and we know that they are not parallel, we just need to calculate the x 
			 * intercept - here taken as t.
			 */
			//y = -this->b/this->a * t - this->c/this->a = m *t + b
			//-this->b/this->a * t - m *t =  b + this->c/this->a
			//(-this->b/this->a - m) *t =  b + this->c/this->a
			t =  (b + this->c/this->a)/(-this->b/this->a - m);
		}

		/**
		 * After we have calculated t, we need to know if this point(t, intercepts[t]) is inside the space
		 * defined by the two points sent as parameters. If they're not between theses parameters, the lines
		 * intersect out of the line segment v1v2.
		 * First we need to set up the boundaries properly.
		 */
		if(upboundx < downboundx)
			std::swap(upboundx, downboundx);

		if(upboundy < downboundy)
			std::swap(upboundy, downboundy);
				
		// Here we use the threshold to reduce numerical issues.
		if((t >= downboundx-threshold) && (t <= upboundx+threshold) && (this->at(t) >= downboundy-threshold) && (this->at(t) <= upboundy+threshold)){
			bool ok=true;
			// Here we avoid to insert too close intercepts.
			for(unsigned int z=0; z<intercepts.size() && ok; z++){
				if(glm::distance(glm::vec2(t, this->at(t)), 
					glm::vec2(intercepts[z], this->at(intercepts[z]))) < threshold)
					ok = false;
			
			}				

			if(ok) intercepts.push_back(t);
		 }

	}

private:

	// Coefficients of the line
	T a, b, c;
};


#endif

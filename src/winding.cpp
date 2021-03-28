#include <winding.hpp>

#include <utility>

namespace winding_number {
namespace {

class BadWindingNumberAlgorithm : public IWindingNumberAlgorithm {
    float isLeft(float x1, float y1, float x2, float y2, float x, float y){
        //Position of (x,y) is found with respect to the line conneting (x1,y1) and (x2,y2). The idea is to use the formula for cross-product. The
        //positions of all three points in question are translated (with their relative positions intact) such that (x1,y1) is located at the origin
        //or (0,0). To make that happen, (x1,y1) is subtracted from its counterparts in (x2,y2) and (x,y). The cross product would be yielded through
        // the formula below. If the direction of the line from (x1,y1) to (x2,y2) is in counter-clockwise direction, then the cross-product would be 
        //positive, else it will be negative. Cross-product would be 0 if the (x,y) lies on the line connecting (x1,y1) and (x2,y2).
        return ((x2 - x1) * (y - y1) - (x - x1) * (y2 - y1));
    }

    //The following implementation works only when edges of the polygon are line segments
    //But, I guess we need to extend it to arcs as well???
    std::optional<int> CalculateWindingNumber2D(float x, float y, poly::Polygon polygon) override {
        // Clearly we can do better...
        if(!(polygon.IsClosed()) || polygon.x_vec_.size()<4 || polygon.y_vec_.size()<4 || polygon.x_vec_.size() != polygon.y_vec_.size()) return std::nullopt;

        //The idea is to use a infinte line originating from the query point (x,y) and going all the way to the right. Every time this line 
        //intersects with an edge of the polygon, two cases are possible
        //1. The intersecting edge of the polygon is directed counterclockwise. This would lead to a positive winding number, hence, it is incremented by 1
        //2. The intersecting edge of the polygon is directed clockwise. This would lead to a negative winding number, hence, it is decremented by 1

        size_t windingNumber_Right = 0, windingNumber_Left = 0;
        //WindingNumber_Right extends the ray from the queried point towards the right
        //WindingNumber_Left extends the ray from the queried point towards the left

        for(size_t edgeIdx = 0; edgeIdx < polygon.x_vec_.size(); edgeIdx++){
            float x1=polygon.x_vec_[edgeIdx];
            float y1=polygon.y_vec_[edgeIdx];
            float x2=polygon.x_vec_[edgeIdx+1];
            float y2=polygon.y_vec_[edgeIdx+1];

            //Source is below the query point in cartesian plane and,
            // Destination of the line is above the query point. So, the line is directed upwards
            if(y1 <= y && y2 >= y){            
                if(isLeft(x1,y1,x2,y2,x,y) > 0)
                    //(x,y) is positioned to the left of the line connecting the points at edgeIdx and edgeIdx+1, with the line directed from the former
                    //to the latter, in the upward direction
                    windingNumber_Right++;
                else if(isLeft(x1,y1,x2,y2,x,y) < 0)
                    windingNumber_Left--;
            }
            //Source is above the query point and,
            //Destination of the line is below the query point. So, the line is directed downwards
            else if(y1 >= y && y2 <= y){         
                if(isLeft(x1,y1,x2,y2,x,y) < 0)
                    //(x,y) is positioned to the left of the line connecting the points at edgeIdx and edgeIdx+1, with the line directed from the former
                    //to the latter, in the downward direction
                    windingNumber_Right--;
                else if(isLeft(x1,y1,x2,y2,x,y) > 0)
                    windingNumber_Left++;
            }
        }
        //The function mentioned above has a linear time complexity with respect to the number of points present in the polygon object

        if(!windingNumber_Right) return windingNumber_Left;
        return windingNumber_Right;
        //error_message("Unimplemented algorithm.");
        //return std::nullopt;
    }
};

}; // namespace

std::unique_ptr<IWindingNumberAlgorithm> IWindingNumberAlgorithm::Create() {
    return std::make_unique<BadWindingNumberAlgorithm>();
}

void IWindingNumberAlgorithm::tolerance(float tolerance) noexcept {
    tolerance_ = tolerance;
}

float IWindingNumberAlgorithm::tolerance() const noexcept {
    return tolerance_;
}

std::string IWindingNumberAlgorithm::error_message() const noexcept {
    return error_message_;
}

void IWindingNumberAlgorithm::error_message(std::string error_message) noexcept {
    error_message_ = std::move(error_message);
}

}  // namespace winding_number

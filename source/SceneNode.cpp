//// Created by Elena Faillace on 26/06/2018.//#include "../include/SceneNode.h"SceneNode::SceneNode() : mParent(nullptr), mChildren() {}void SceneNode::attachChild(Ptr child) {    child->mParent = this;    //linear time and pass the ownership    mChildren.push_back(std::move(child));}SceneNode::Ptr SceneNode::detachChild(const SceneNode &node) {    //find_if return iterator to found element    auto found = std::find_if(mChildren.begin(),mChildren.end(),                              //ptr is a pointer to sceneNode                              [&](Ptr& ptr)-> bool { return ptr.get() == &node; });    //checks if found, end is an empty(?) iterator at the end o the array    assert(found != mChildren.end());    //found is an iterator poiting to Ptr    Ptr result = std::move(*found);    result->mParent = nullptr;    mChildren.erase(found);    return result;}void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const {    //combines the parent's absolute transform with the current node's relative one    states.transform *= getTransform(); //contains the absolute world transform    drawCurrent(target, states);    //use iterators if it doesn't work    for ( const Ptr &child : mChildren )        child->draw(target, states);}void SceneNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {    //do nothing by default}
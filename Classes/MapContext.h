//
//  MapContext.h
//  NewYorkAtoB
//
//  Created by teivaz on 1/19/16.
//
//

#pragma once

// Context class
// We can theoretically switch maps by only changing the loader

class MapContext{
public:
    class MapLoader* Loader;
    
    MapContext():Loader(nullptr){}
};

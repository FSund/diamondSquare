#ifndef PRINTIMAGE_H
#define PRINTIMAGE_H

// From here: https://code.google.com/p/fractalterraingeneration/downloads/detail?name=DiamondSquare.1.0.cpp

#include <vector>
#include <string>
#include <fstream>

struct color{
    //v[0]=red, v[1]=green, v[2]=blue
    unsigned char v[3];

    color(unsigned char r, unsigned char g, unsigned char b){
        v[0]=r;
        v[1]=g;
        v[2]=b;
    }
};

color linearInterpolation(color c1, color c2, double value){
    color tcolor(0,0,0);

    for (int g = 0; g < 3; g++) {
        if (c1.v[g] > c2.v[g])
            tcolor.v[g] = c2.v[g] + (unsigned char)(double(c1.v[g] - c2.v[g])*value);

        else
            tcolor.v[g] = c1.v[g] + (unsigned char)(double(c2.v[g] - c1.v[g])*value);
    }

    return tcolor;
}

void printMap(std::vector<std::vector<double> > map, std::string filename) {
//set up some variables
    double min = 0;
    double max = 0;
    for (uint i = 0; i < map.size(); i++) {
        for (uint j = 0; j < map[i].size(); j++) {
            if (map[i][j] > max) max = map[i][j];
            if (map[i][j] < min) min = map[i][j];
        }
    }
    double diff = max - min,
          flood = 0.5,//flood level
          mount = 0.85;//mountain level

    flood *= diff;
    mount *= diff;

    int i,j,k;

    int hgrid = map.size();
    int vgrid = map[0].size();

    //these can be changed for interesting results
    color landlow(0,64,0),
          landhigh(116,182,133),
          waterlow(55,0,0),
          waterhigh(106,53,0),
          mountlow(147,157,167),
          mounthigh(226,223,216);

//3.0 output to file
    //3.1 Begin the file
    //3.1.1 open output file
    std::ofstream out;
    out.open(filename, std::ofstream::binary);
    if (!(out.is_open())){
        std::cout << "Target file opening error" << std::endl;
        exit(0);
    }

    //3.1.2 copy the header
        //3.1.2.1 magic number
        out.put(char(66));
        out.put(char(77));

        //3.1.2.2 filsize/unused space
        for (i = 0; i < 8; i++)
            out.put(char(0));

        //3.1.2.3 data offset
        out.put(char(54));

        //3.1.2.4 unused space
        for (i = 0; i < 3; i++)
            out.put(char(0));

        //3.1.2.5 header size
        out.put(char(40));

        //3.1.2.6 unused space
        for (i = 0; i < 3; i++)
            out.put(char(0));

        //3.1.2.7 file width (trickier)
        out.put(char(hgrid%256));
        out.put(char((hgrid>>8)%256));
        out.put(char((hgrid>>16)%256));
        out.put(char((hgrid>>24)%256));

        //3.1.2.8 file height (trickier)
        out.put(char(vgrid%256));
        out.put(char((vgrid>>8)%256));
        out.put(char((vgrid>>16)%256));
        out.put(char((vgrid>>24)%256));

        //3.1.2.9 color planes
        out.put(char(1));
        out.put(char(0));

        //3.1.2.10 bit depth
        out.put(char(24));

        //3.1.2.11 the rest
        for (i=0;i<25;i++)
            out.put(char(0));

    //3.2 put in the elements of the array
    color newcolor(0,0,0);
    for (i = (vgrid-1); i >= 0; i--) {//bitmaps start with the bottom row, and work their way up...
        for (j = 0; j < hgrid; j++) {//...but still go left to right
            map[j][i] -= min;
            //if this point is below the floodline...
            if (map[j][i] < flood)
                newcolor = linearInterpolation(waterlow, waterhigh, map[j][i]/flood);

            //if this is above the mountain line...
            else if (map[j][i] > mount)
                newcolor = linearInterpolation(mountlow, mounthigh, (map[j][i] - mount)/(diff - mount));

            //if this is regular land
            else
                newcolor = linearInterpolation(landlow, landhigh, (map[j][i] - flood)/(mount-flood));

            out.put(char(newcolor.v[0]));//blue
            out.put(char(newcolor.v[1]));//green
            out.put(char(newcolor.v[2]));//red
        }
        //round off the row
        for (k = 0; k < (hgrid%4); k++)
            out.put(char(0));
    }

    //3.3 end the file
    out.close();
}

#endif // PRINTIMAGE_H

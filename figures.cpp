#include "figures.h"
Figures::Figures()
{
}
Figures::~Figures()
{
}
std::vector<std::vector<int> > Figures::getFigure(int figure, int state) {
    if ( figure < 1 ) figure = 1;
    else if ( figure > 7 ) figure = 7;
    if ( state < 1 ) state = 1;
    else if ( state > 4 ) state = 4;
    std::vector<std::vector<int> > v;
    for ( int i = 0; i < 4; i++ ) {
        std::vector<int> a;
        for ( int j = 0; j < 4; j++ ) {
            if ( figure == 1 ) {
                    a.push_back(Figures::figure1_1[i][j]);
                } else if ( figure == 2 ) {
                    if ( state == 1 ) {
                        a.push_back(Figures::figure2_1[i][j]);
                    } else if ( state == 2 ) {
                        a.push_back(Figures::figure2_2[i][j]);
                    } else if ( state == 3 ) {
                        a.push_back(Figures::figure2_3[i][j]);
                    } else if ( state == 4 ) {
                        a.push_back(Figures::figure2_4[i][j]);
                    }
                } else if ( figure == 3 ) {
                    if ( state == 1 ) {
                        a.push_back(Figures::figure3_1[i][j]);
                    } else if ( state == 2 ) {
                        a.push_back(Figures::figure3_2[i][j]);
                    } else if ( state == 3 ) {
                        a.push_back(Figures::figure3_3[i][j]);
                    } else if ( state == 4 ) {
                        a.push_back(Figures::figure3_4[i][j]);
                    }
                } else if ( figure == 4 ) {
                    if ( state == 1 ) {
                        a.push_back(Figures::figure4_1[i][j]);
                    } else if ( state == 2 ) {
                        a.push_back(Figures::figure4_2[i][j]);
                    } else if ( state == 3 ) {
                        a.push_back(Figures::figure4_3[i][j]);
                    } else if ( state == 4 ) {
                        a.push_back(Figures::figure4_4[i][j]);
                    }
                } else if ( figure == 5 ) {
                    if ( state == 1 ) {
                        a.push_back(Figures::figure5_1[i][j]);
                    } else if ( state == 2 ) {
                        a.push_back(Figures::figure5_2[i][j]);
                    } else if ( state == 3 ) {
                        a.push_back(Figures::figure5_3[i][j]);
                    } else if ( state == 4 ) {
                        a.push_back(Figures::figure5_4[i][j]);
                    }
                } else if ( figure == 6 ) {
                    if ( state == 1 ) {
                        a.push_back(Figures::figure6_1[i][j]);
                    } else if ( state == 2 ) {
                        a.push_back(Figures::figure6_2[i][j]);
                    } else if ( state == 3 ) {
                        a.push_back(Figures::figure6_3[i][j]);
                    } else if ( state == 4 ) {
                        a.push_back(Figures::figure6_4[i][j]);
                        }
                } else if ( figure == 7 ) {
                    if ( state == 1 ) {
                        a.push_back(Figures::figure7_1[i][j]);
                    } else if ( state == 2 ) {
                        a.push_back(Figures::figure7_2[i][j]);
                    } else if ( state == 3 ) {
                        a.push_back(Figures::figure7_1[i][j]);
                    } else if ( state == 4 ) {
                        a.push_back(Figures::figure7_2[i][j]);
                    }
                }
        }
        v.push_back(a);
    }
    return v;
}

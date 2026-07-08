#include "scene.h"
#include<ctime>

/**
 * @brief Main function
 * 
 * Main function for the Raytracing Project:  
 * Creates a scene loads the settings and renders the images.
*/
int main(){


    Scene scn;
    scn.load_settings();
    
    double time_before_loop_begins = std::time(NULL);
    
    scn.render_imag();

    double time_after_loop_ends = std::time(NULL);

    double time_diff = time_after_loop_ends - time_before_loop_begins;
    cout << "Render time : " << time_diff << " seconds."<<endl;
}

#include <ros/ros.h>
#include <GPS_control_functions.hpp>

std::vector<gnc_api_waypoint> waypointlist;
gnc_api_waypoint nextwp;

int move(float lat,float lon,float alt){

    nextwp.lat = lat;
    nextwp.lon = lon;
    nextwp.alt = alt;
    waypointlist.push_back(nextwp);

	return 0;
 }


int start_mission(){

    ros::Rate rate(2.0);
    wait4connect();   

	set_mode("GUIDED");	

	//create local reference frame 
	initialize_local_frame();

	arm();

	//request takeoff
	takeoff(1);
	


	auto globel=get_position();

	move(globel.lat,globel.lon,2);
	move(globel.lat + 0.00003 ,globel.lon + 0.0 , 2);
	move(globel.lat + 0.00003 ,globel.lon + 0.00003,2);
	move(globel.lat + 0.0 ,globel.lon + 0.00003 ,2);
	move(globel.lat,globel.lon,2);

    
    int counter = 0;
    //wait for position information
    while(ros::ok()) 
    {

	if(counter < waypointlist.size())
	{
		set_waypoint(waypointlist[counter].lat,waypointlist[counter].lon,waypointlist[counter].alt);
		ROS_INFO("GPS position sent: [%f %f %f]", waypointlist[counter].lat, waypointlist[counter].lon, waypointlist[counter].alt);
		// counter++;
		// sleep(4);
		while (true)	
		{		
		if (check_position(waypointlist[counter].lat, waypointlist[counter].lon)== 1)
		{
			ROS_INFO("ok");
			counter++;
			break;
		}
		}
		sleep(5);	
	}
	else
	{
		land();
	} 
       
    }

return 0;
}





int main(int argc, char **argv)
{
	ros::init(argc, argv, "gnc_node");
	ros::NodeHandle gnc_node("~");
	//initialize control publisher/subscribers
	init_publisher_subscriber(gnc_node);

	start_mission();
    return 0;
}

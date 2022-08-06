#include <iostream>
#include <tinyxml2.h>
#include <iomanip>
#include <regex>
#include <boost/regex.hpp>

#include <ros/package.h>
#include <rospack/rospack.h>

int main(int argc, char** argv )
{
    std::string package_path = ros::package::getPath("xacro");
    std::string relative_robot_path = "/test/robots/pr2/pr2.urdf.xacro"; 
    std::string full_path = package_path + relative_robot_path;
    
    using namespace tinyxml2;
    {
        XMLDocument doc;
        tinyxml2::XMLError eResult = doc.LoadFile( full_path.c_str() );
        if (eResult != tinyxml2::XML_SUCCESS) return false;


        std::cout << "Loaded the doc" << std::endl;
        std::cout << " children? " << doc.NoChildren() << std::endl;
        XMLNode * child = doc.FirstChildElement("robot");
        const char * robot_name = child->ToElement()->Attribute("name");
        std::cout << robot_name << std::endl;

        XMLNode * node = child->FirstChildElement("xacro:include"); 
        const char * include_path_c_str = node->ToElement()->Attribute("filename");
        std::string include_path = std::string(node->ToElement()->Attribute("filename"));
        // include_path.find()
        // std::cout << include_path_c_str << std::endl;
        // std::cmatch cm;    // same as std::match_results<const char*> cm;
        // // std::regex e ("\\$\\(find .*\\)");
        // std::regex e ("(find)");
        // std::regex_match (include_path_c_str, cm, e);

        
        
        XMLPrinter printer;
        node->Accept( &printer );
        std::cout << printer.CStr() << std::endl;

        if (child == nullptr)
        {
            std::cout << "nullptr at first child" << std::endl;
            return 1;
        }

    }

    {
        std::string text("$(find xacro)/test/robots/pr2/torso_v0/torso.urdf.xacro");
        const char* pattern ="\\$\\(\\bfind\\b.*\\)";

        boost::regex ip_regex(pattern);
        boost::sregex_iterator it(text.begin(), text.end(), ip_regex);
        boost::sregex_iterator end;
        std::string find_prefix = "$(find "; 
        for (; it != end; ++it) {
            std::string find_package = it->str();
            std::cout << find_package << std::endl;
            std::string res = ros::package::command(find_package.substr(2, find_package.size()-3));
            std::cout << "RES : " << res << std::endl;
            // std::string package_name = find_package.substr(find_prefix.size(), 
            //                                           find_package.size() - find_prefix.size()-1);
            
            // std::cout << " this is the package " <<  package_name << std::endl;
            // std::string found_package_path = ros::package::getPath(package_name);
            // if (found_package_path.empty())
            // {
            //     throw std::runtime_error("ROS package not found");
            // }

        }
    }
    
    
    std::cout << "Finishing" << std::endl;
    return 1;
}


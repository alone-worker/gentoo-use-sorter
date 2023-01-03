#include <iostream>
#include <filesystem>
#include <fstream>

int main()
{
    std::filesystem::create_directories("backup_portage_use");
    std::filesystem::create_directories("new_portage_use");

    std::string line;
    std::ifstream fs_stream;
    std::fstream fs_new_file_stream;
    
    std::string prefix;

    for(auto& p: std::filesystem::directory_iterator("/etc/portage/package.use/"))
    {
        std::string this_file = std::filesystem::path(p).filename();

        if (this_file[0] == '.')
        {
            continue;
        }
        
        std::filesystem::copy_file(p, std::filesystem::current_path()
        .append("backup_portage_use")
        .append(this_file), std::filesystem::copy_options::overwrite_existing);

        fs_stream.open(p.path());

        if (fs_stream.is_open())
        {
            prefix = "";
            while (getline(fs_stream, line))
            {
                for(int i = 0; i < line.length(); ++i) 
                {
                    if (line[i] == '#')
                    {
                        prefix = "";
                        break;
                    }

                    if (line[i] == '/')
                    {
                        break;
                    }

                    if (line[i] == '>' || line[i] == '=')
                    {
                        continue;
                    }

                    prefix = prefix + line[i];
                }

                if (prefix == "*") 
                {
                    prefix = "all";
                }

                fs_new_file_stream.open(std::filesystem::current_path()
                .append("new_portage_use")
                .append(prefix), std::ios::app);

                fs_new_file_stream << line << std::endl;
                prefix = "";
                fs_new_file_stream.close();
            }
       }

        fs_stream.close();
    }

    return 0;
}
#include "RBtree.hpp"

std::string mtolower(std::string str)
{
    for (size_t i = 0; i < str.size(); ++i)
    {
        str[i] = tolower(str[i]);
    }
    return str;
}

int main()
{
    std::string input;
    TRBTree tree;
    while (std::cin >> input)
    {
        if (input == "+")
        {
            uint64_t value;
            std::string key;
            std::cin >> key >> value;
            if (tree.Insert(mtolower(key), value))
            {
                std::cout << "OK\n";
            }
            else
            {
                std::cout << "Exist\n";
            }
        }
        else if (input == "-")
        {
            std::string key;
            std::cin >> key;
            if (tree.Erase(mtolower(key)))
            {
                std::cout << "OK\n";
            }
            else
            {
                std::cout << "NoSuchWord\n";
            }
        }
        else if (input == "!")
        {
            std::string action;
            std::string path;
            std::cin >> action >> path;
            if (action == "Save")
            {
                std::ofstream file;
                try 
                {
                    file.open(path);
                }
                catch (std::exception& ex)
                {
                    std::cout << "ERROR: " << ex.what() << "\n";
                }
                tree.Save(file);
            }
            else if (action == "Load")
            {
                std::ifstream file;
                try 
                {
                    file.open(path);
                }
                catch (std::exception& ex)
                {
                    std::cout << "ERROR: " << ex.what() << "\n";
                }
                tree.Load(file);
            }
            std::cout << "OK\n";
        }
        else
        {
            TNode *FoundNode = tree.FindNode(mtolower(input));
            if (FoundNode == tree.GetNil())
            {
                std::cout << "NoSuchWord\n";
            }
            else
            {
                std::cout << "OK: " << FoundNode->value << "\n";
            }
        }
    }
}

// int main()
// {
//     std::string input;
//     TRBTree tree;
//     while (std::cin >> input)
//     {
//         if (input == "+")
//         {
//             uint64_t value;
//             std::string key;
//             std::cin >> key >> value;
//             if (tree.Insert(key, value))
//             {
//                 std::cout << "OK\n";
//             }
//             else
//             {
//                 std::cout << "Exist\n";
//             }
//         }
//         else if (input == "-")
//         {
//             std::string key;
//             std::cin >> key;
//             if (tree.Erase(key))
//             {
//                 std::cout << "OK\n";
//             }
//             else
//             {
//                 std::cout << "NoSuchWord\n";
//             }
//         }
//         else if (input == "!")
//         {
//             std::string action;
//             std::string path;
//             std::cin >> action >> path;
//             if (action == "Save")
//             {
//                 std::ofstream file;
//                 try 
//                 {
//                     file.open(path);
//                 }
//                 catch (std::exception& ex)
//                 {
//                     std::cout << "ERROR: " << ex.what() << "\n";
//                 }
//                 tree.Save(file);
//             }
//             else if (action == "Load")
//             {
//                 std::ifstream file;
//                 try 
//                 {
//                     file.open(path);
//                 }
//                 catch (std::exception& ex)
//                 {
//                     std::cout << "ERROR: " << ex.what() << "\n";
//                 }
//                 tree.Load(file);
//             }
//             std::cout << "OK\n";
//         }
//         else
//         {
//             TNode *FoundNode = tree.FindNode(input);
//             if (FoundNode == tree.GetNil())
//             {
//                 std::cout << "NoSuchWord\n";
//             }
//             else
//             {
//                 std::cout << "OK: " << FoundNode->value << "\n";
//             }
//         }
//     }
// }
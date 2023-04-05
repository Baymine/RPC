// #include "openai.hpp"
#include <iostream>
#include <map>
#include <string>
#include "include/openai/openai.hpp"

int main() {
    // openai::start(); // Will use the api key provided by `OPENAI_API_KEY` environment variable
    openai::start(); // Or you can handle it yourself

    // std::string prompt = "Write an buble sort in C++";
    // auto completion = openai::completion().create(R"({
    //     "model": "text-davinci-003",
    //     "prompt": prompt,
    //     "max_tokens": 7,
    //     "temperature": 0
    // })"_json); // Using user-defined (raw) string literals
    
    // std::string prompt = R"({
    //     "model": "text-davinci-003",
    //     "prompt": "Say this is a test",
    //     "max_tokens": 7,
    //     "temperature": 0
    // })";

    std::string t = R"("Say this is a test")";

    nlohmann::json prompt = {
        {"model", "text-davinci-003"}, 
        {"prompt", t}, 
        {"max_tokens", 7},
        {"temperature", 0}
    };

    auto completion = openai::completion().create(prompt);

    // auto completion = openai::completion().create(nlohmann::json::parse(prompt));



    std::cout << "The text: " << completion["choices"][0]["text"] << std::endl;
    std::cout << "-----------------------------------------" << std::endl;

    std::cout << "Response is:\n" << completion.dump(2) << '\n'; 


    // auto image = openai::image().create({
    //     { "prompt", "A cute koala playing the violin"},
    //     { "n", 1 },
    //     { "size", "512x512" }
    // }); // Using initializer lists
    // std::cout << "Image URL is: " << image["data"][0]["url"] << '\n'; 
}
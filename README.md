# Cave Engine's Standard Libs
Here you'll find a set of my own implementations to some Standard (std:: and others) libraries in C++. Intended to be extended (internally in the studio) to use in Cave Engine.

# FYI
## Why did I implement all this?
I've reached a point in **Cave Engine** where I really need to have a bit more control over all the data types that I use. Specially when it comes to serialization. Having the freedom to change how it allocates memory is also important. So here it goes.

**Compatibility Note:** While some of cave's std implementation APIs are pretty much identical to the standard ones (like `cave::String` and `std::string`), I am **NOT** keeping them completely compatible. Some methods were simplified and others changed (or added). Also notice that it uses **Camel Case naming**, to follow Uniday Studio's internal naming conventions. So vector's `push_back`, for example, is `pushBack`.


# Project Progress
I'd like to write my own implementation of the most used `std::` classes in Cave and then move on to also write my own **math** class (to eventually replace the use of `glm`). Here is the current development status:

| **Class (std):**| **Class (cave):** | **Status** |
|-----------------|-------------------|------------|
| `std::string`   | `cave::String`    |  **DONE**  |
| `std::hash<std::string>`   | `std::hash<cave::String>`    |  **DONE**  |
| `std::vector<T>`| `cave::Vector<T>` |  **DONE**  |
| `std::list<T>`  | `cave::List<T>`   |  **DONE**  |
| `std::pair<T1, T2>`  | `cave::Pair<T1, T2>`   |  **DONE**  |
| `std::unordered_map<K, V>`   | `cave::HashMap<K, V>`    |  **WORKING**, *but missing rehash.*  |
| `std::map<K, V>`   | `cave::Map<K, V>`    |  *Nope! Use HashMap instead.*  |

# Contributing
Yes! If you find a bug, room for optimization or anything, feel free to make a Pull Request or **Open an Issue**! I decided to make this part of the code Open Source exactly to both help the community and also get helped, since it's a core element of the engine.

But please, keep the naming conventions and code style guides and **write Unit Tests** to the new stuff you add.

## [Join our Discord Server!](https://bit.ly/discord-uniday-us)
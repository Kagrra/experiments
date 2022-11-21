## any_callable
I really like working with `std::function` but during embedded developement I would often like something faster. Also not allocating would be a plus. So I made `any_callable` as a kinda type-erasing alternative with predictable behavior and static storage.


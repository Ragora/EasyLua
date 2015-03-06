function easyLuaMultiParamMultiReturnTest(first, second, third)
    print("Got First parameter: " .. first)
    print("Got second Parameter: " .. second)
    print("Got third parameter: " .. third)
    
    return 4, 3.14, "Test"
end

function easyLuaSingleParamSingleReturn(first)
    --print("WAT")
    --print(first["Five"])
    for key, value in pairs(first["Five"]["Eight"]) do
        print("Key ".. key)
        print("Value: ")
        print(value)
    end
    
    return "Blah"
end

function easyLuaSingleIntegerReturnTest(first, second, third)
    print("Got First parameter: " .. first)
    print("Got second Parameter: " .. second)
    print("Got third parameter: " .. third)

    return 1337
end

function checkValue(expected, input, message)
    if not (input == expected) then
        print(message)
        return false
    end

    return true
end

function checkType(expected, input, message)
    if not (type(input) == expected) then
        print(message)
        return false
    end

    return true
end

function checkFloat(expected, threshold, message)
    local min = expected - threshold
    local max = expected + threshold

    if not (expected >= min and expected <= max) then
        print(message)
        return false
    end

    return true
end

function printTable(table)
    for key, value in pairs(table) do
        if type(value) == "table" then
            print("Sub Table------")
            print(value)
            printTable(value)
        else
            print(table)
            print(key .. " = " .. value)
        end
    end
end

function easyLuaHLTable(number, table)
    --if not checkValue(3.14, number, "First number was not 3.14") then
    --    return false
    --end

    return true
end

function easyLuaMultiParamMultiReturnTest(one, two, three)

    print("Table One----")
    printTable(one)
    print("----------------")
    print(" ")

    print("Two = " .. two)
    print("Three = " .. three)
    print("Four = " .. four)
    print("Five = " .. five)
    print("Six = " .. six)
    print(" ")

    print("Table Two----")
    printTable(seven)
    print("----------------")
    print(" ")

    print("Table Three----")
    printTable(eight)
    print("----------------")
    print(" ")

    print("Nine = " .. nine)

    print("Table Four----")
    printTable(ten)
    print("----------------")
    print(" ")

    return 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
end


function easyLuaSubTables(one, two, three, four, five, six, seven, eight, nine, ten, eleven)
    -- One should be a table
    if not checkType("table", one, "First parameter is not a table.") then
        return false
    end

    -- Then one.Test should equal 3
    if not checkValue(3, one.Test, "First table Test property does not equal 3.") then
        return false
    end

    -- Two should simply equal "One"
    if not checkValue("One", two, "Second parameter value is not 'One'") then
        return false
    end

    -- Three should simply equal 2
    if not checkValue(2, three, "Third parameter is not 2.") then
        return false
    end

    -- Four should simply equal "Three"
    if not checkValue("Three", four, "Fourth parameter is not 'Three'.") then
        return false
    end

    -- Five should simply equal 4.12
    if not checkFloat(4.12, 0.1, five, "Fifth parameter is not 4.12.") then
        return false
    end

    -- Six should simply equal "Five"
    if not checkValue("Five", six, "Sixth parameter is not 'Five'.") then
        return false
    end

    -- Seven should be a table
    print(seven)
    local result = checkType("table", seven, "Seventh parameter is not a table.")
    --print(result)

    --if not result then
    --    return false
    --end

    return true
end

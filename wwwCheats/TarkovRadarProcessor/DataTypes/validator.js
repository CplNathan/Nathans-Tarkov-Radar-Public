/* Validates an input JSON object to the comparitor object removing invalid fields, returns the sourceObject transferred into the compareObject fields */ 
function validatePacket(sourceObject, compareObject)
{
    // Come to think of it this does not really work, as if a packet contains an array or an object you can just add extra junk to that instead and itterating it is time consuming... rip
    let missingFields = 0;
    let resultObject = {

    };

    for (object in compareObject)
    {
        if (compareObject[object] === undefined)
        {
            missingFields += 1;
        }
        else
        {
            if (compareObject[object] === sourceObject[object]) // Ensure that the type is the same.
                resultObject[object] = sourceObject[object]
        }
    }

    return resultObject;
}

module.exports = {
    validatePacket: validatePacket
}
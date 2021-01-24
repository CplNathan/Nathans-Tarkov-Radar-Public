class Packet {
    buildData()
    {
        return {};
    }

    constructor(data) {
        this.rawData = data;
        this.dataStructure = this.buildData();
    }
}

class Response extends Packet {
    constructor(data) {
        super(data);

        this.rawData = this.buildData(data);
        this.dataStructure = this.buildData(data);
    }
}

class RegisterObject extends Packet
{
    buildData()
    {
        return {
            "Passcode": "",
            "isHost": false,
            "isClient": false
        }
    }
}

class RegisterResponse extends Response
{
    buildData(data)
    {
        return {
            "success": data
        }
    }
}

class NewPlayer extends Packet
{
    buildData()
    {
        return {
            "name": "",
            "id": "",
            "coordinates": {
                "x": 0,
                "y": 0,
                "z": 0
            },
            "viewangle": {
                "x": 0,
                "y": 0
            },
            "islocal": false,
            "isscav": false,
            "isplayerscav": false,
            "groupid": "0",
            "membertype": 0,
            "weapon": "None"
        }
    }
}

class UpdatePlayer extends Packet
{
    buildData()
    {
        return {
            "id": "",
            "coordinates": {
                "x": 0,
                "y": 0,
                "z": 0
            },
            "viewangle": {
                "x": 0,
                "y": 0
            },
            "weapon": "None"
        }
    }
}

class DeletePlayer extends Packet
{
    buildData()
    {
        return {
            "id": ""
        }
    }
}

class AddLoot extends Packet
{
    buildData()
    {
        return {
            "name": "",
            "coordinates": {
                "x": 0,
                "y": 0,
                "z": 0
            },
            "corpse": false,
            "highvalue": false,
            "signature": ""
        }
    }
}

class DeleteLoot extends Packet
{
    buildData()
    {
        return {
            "signature": ""
        }
    }
}

class AddExfil extends Packet
{
    buildData()
    {
        return {
            "id": "",
            "hint": "",
            "status": 0,
            "openms": 0,
            "chance": 0,
            "coordinates": {
                "x": 0,
                "y": 0,
                "z": 0
            }
        }
    }
}

class UpdateExfil extends Packet
{
    buildData()
    {
        return {
            "id": "",
            "status": 0
        }
    }
}

module.exports = {
    Packet: Packet,
    RegisterObject: RegisterObject,
    RegisterResponse: RegisterResponse,
    NewPlayer: NewPlayer,
    UpdatePlayer: UpdatePlayer,
    DeletePlayer: DeletePlayer,
    AddLoot: AddLoot,
    DeleteLoot: DeleteLoot,
    AddExfil: AddExfil,
    UpdateExfil: UpdateExfil
}
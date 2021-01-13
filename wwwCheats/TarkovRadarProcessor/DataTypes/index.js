const validator = require("./validator.js");
const objects = require("./BaseObject.js");

module.exports = Object.assign(validator, {dataTypes: objects});
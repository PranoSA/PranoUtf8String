const ad = require('./build/Release/addon.node')

// Path: call_hello_world.js

const r = ad.add(4, 9, 14, (err, result) => {
    if (err) {
        console.error(err)
        return
    }
    console.log(result)
    }
)

const obj = new ad.MyObject(10)

//console.log(obj.plusOne().plusOne().plusOne().value)
console.log(new ad.MyObject(obj.plusOne()).plusOne())

console.log(r)
// Path: call_hello_world.js
console.log("Finished")
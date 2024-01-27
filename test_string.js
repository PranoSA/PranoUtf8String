const pranoutf8 = require('./build/Release/addon.node')

console.log(pranoutf8)

console.log(new pranoutf8.PranoUtf8String('Hello World!'))

console.log( new pranoutf8.PranoUtf8String('Hello World!').toString())

let h = new pranoutf8.PranoUtf8String('Hello World!').split(1,7)

console.log(h.toString())

console.log(new pranoutf8.PranoUtf8String('Hello World!').split(0,4).toString())

console.log(new pranoutf8.PranoUtf8String('Hello World!').split(0,2).toString())

console.log("Correct String?    " + new pranoutf8.PranoUtf8String('😊').toString())
console.log("Correct String?    " + new pranoutf8.PranoUtf8String('😁').toString())

console.log(new pranoutf8.PranoUtf8String('😊😊😊😊😊😊').split(2,4).toString())

console.log(new pranoutf8.PranoUtf8String('😀😃😄😁😆😅😂🤣🥲🥹☺️😊😇🙂🙃😉😌😍🥰😘😗😙').split(5,15).toString())

console.log(new pranoutf8.PranoUtf8String('😀😃😄😁😆😅😂🤣🥲🥹☺️😊😇🙂🙃😉😌😍🥰😘😗😙').UnicodeValues())

let unicode_array = new pranoutf8.PranoUtf8String('😀😃😄😁😆😅😂🤣🥲🥹☺️😊😇🙂🙃😉😌😍🥰😘😗😙').UnicodeValues()

//Convert Each element to hex string
let hex_array = unicode_array.map((element, index) => {
    return element.toString(16)
})

console.log(hex_array)

let combine_string = "combined with" + new pranoutf8.PranoUtf8String('😀😃😄😁😆😅😂🤣🥲🥹☺️😊😇🙂🙃😉😌😍🥰😘😗😙')

console.log(combine_string)

console.log(hex_array + new pranoutf8.PranoUtf8String('😀😃😄😁😆😅😂🤣🥲🥹☺️😊😇🙂🙃😉😌😍🥰😘😗😙'))


// Path: test_string.js

const pranoutf8 = require('./build/Release/addon.node')

console.log(pranoutf8)

console.log(new pranoutf8.PranoUtf8String('Hello World!'))

console.log( new pranoutf8.PranoUtf8String('Hello World!').toString())

let h = new pranoutf8.PranoUtf8String('Hello World!').split(1,7)

console.log(h.toString())

console.log(new pranoutf8.PranoUtf8String('Hello World!').split(0,4).toString())

console.log(new pranoutf8.PranoUtf8String('Hello World!').split(0,2).toString())

console.log(new pranoutf8.PranoUtf8String('😊').toString())

console.log(new pranoutf8.PranoUtf8String('😊😊😊😊😊😊').split(2,4).toString())

console.log(new pranoutf8.PranoUtf8String('😀😃😄😁😆😅😂🤣🥲🥹☺️😊😇🙂🙃😉😌😍🥰😘😗😙').split(5,15).toString())

console.log(new pranoutf8.PranoUtf8String('😀😃😄😁😆😅😂🤣🥲🥹☺️😊😇🙂🙃😉😌😍🥰😘😗😙').UnicodeValues())

let unicode_array = new pranoutf8.PranoUtf8String('😀😃😄😁😆😅😂🤣🥲🥹☺️😊😇🙂🙃😉😌😍🥰😘😗😙').UnicodeValues()

//Convert Each element to hex string
unicode_array.forEach((element, index) => {
    console.log(element.toString(16))
})

// Path: test_string.js

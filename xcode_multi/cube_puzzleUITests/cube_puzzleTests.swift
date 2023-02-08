//
//  cube_puzzleTests.swift
//  cube_puzzleTests
//
//  Created by Tom Arlt on 05.02.23.
//

import XCTest

final class cube_puzzleTests: XCTestCase {
    
    var app = XCUIApplication()

    override func setUpWithError() throws {
        app = XCUIApplication()
        app.launchArguments = ["enable-screenshot-data"]
        // Put setup code here. This method is called before the invocation of each test method in the class.
    }

    override func tearDownWithError() throws {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
    }
    
    func testGenerateScreenshots() throws {
        app.launch()
        
        takeScreenshot(named: "startscreen")
        
        app.tap()
        
        takeScreenshot(named: "level_selector")
        
        XCTAssert(true)
    }
    
    func takeScreenshot(named name: String) {
        // Take the screenshot
        let fullScreenshot = XCUIScreen.main.screenshot()
        
        // Create a new attachment to save our screenshot
        // and give it a name consisting of the "named"
        // parameter and the device name, so we can find
        // it later.
        let screenshotAttachment = XCTAttachment(
            uniformTypeIdentifier: "public.png",
            name: "Screenshot-\(UIDevice.current.name)-\(name).png",
            payload: fullScreenshot.pngRepresentation,
            userInfo: nil)
            
        // Usually Xcode will delete attachments after
        // the test has run; we don't want that!
        screenshotAttachment.lifetime = .keepAlways
        
        // Add the attachment to the test log,
        // so we can retrieve it later
        add(screenshotAttachment)
    }


}

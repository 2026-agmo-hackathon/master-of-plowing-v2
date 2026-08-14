var mockCloud = angular.module('mockCloud', [
    'ngCookies',
    'ui.router',
    'ui.router.state.events',
    'ngFileUpload'
]).run(function($rootScope, $location,$cookies,service , $window) {
    console.log("Mock cloud App Running!!!");
});


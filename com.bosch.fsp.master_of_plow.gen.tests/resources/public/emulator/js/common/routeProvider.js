mockCloud.config(function($stateProvider, $urlRouterProvider) {
    $urlRouterProvider.otherwise('/main');

    $stateProvider
        .state('main', {
            url: '/main',
            templateUrl: 'main.html',
            controller: 'MainController as vm'
        })

        .state('main.dashboard', {
            url: '/dashboard',
            // template:'<h1>Hello Cloud</h1>', ahah ?
            templateUrl: 'dashboard.html',
            resolve: {
                uploadedFiles: function(service) {
                    return service.getUploadedfiles();
                },
                downloadedFiles: function(service) {
                    return service.getDownloadedfiles();
                }
            },
            controller: 'DashboardController as vm'
        })

        .state('main.download', {
            url: '/push',
            templateUrl: 'datadownload.html',
            controller: 'DataDownloadController as vm'
        })
});
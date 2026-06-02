pipeline {
	agent { dockerfile true }
	options {
		buildDiscarder(logRotator(numToKeepStr: "50"))
	}
	stages {
		stage('Ensure clean build') {
			steps {
				sh 'git clean -dfx'
			}
		}
		stage('Submodules') {
			steps {
				sh 'git submodule update --init --recursive'
			}
		}
		stage('Build All') {
			steps {
				sh './scripts/build.sh'
			}
		}
		stage('Test') {
			steps {
				sh 'ctest --test-dir builds/host'
			}
			post {
				always {
					xunit (
					  tools: [ GoogleTest(pattern: 'builds/host/testresults/*.xml') ]
					)
				}
			}
		}
	}
}

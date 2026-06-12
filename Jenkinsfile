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
		stage('Create builds directory') {
			steps {
				sh 'mkdir builds'
			}
		}
        stage('Build & Test') {
            parallel {
                stage('Host') {
                    stages {
                        stage('Build tests and demos') {
                            steps {
                                sh 'cmake -DCMAKE_BUILD_TYPE=Debug -B builds/host && cmake --build builds/host'
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
                stage('Build for Flash') {
                    steps {
                        sh 'cmake -DSRAM_ONLY=OFF -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=cmake_toolchains/arm_cortex_m4.cmake -B builds/target_flash && cmake --build builds/target_flash'
                        sh './scripts/size.sh flash > size_flash.txt'
                    }
                    post {
                        success {
                            archiveArtifacts artifacts: 'size_flash.txt'
                        }
                    }
                }
                stage('Build for SRAM') {
                    steps {
                        sh 'cmake -DSRAM_ONLY=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=cmake_toolchains/arm_cortex_m4.cmake -B builds/target_sram && cmake --build builds/target_sram'
                        sh './scripts/size.sh sram > size_sram.txt'
                    }
                    post {
                        success {
                            archiveArtifacts artifacts: 'size_sram.txt'
                        }
                    }
                }
            }
        }
	}
}

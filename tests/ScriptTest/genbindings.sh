CURRENT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $CURRENT_DIR/../../../cocos-engine/native
python tools/tojs/genbindings.py --config $CURRENT_DIR/chassis.ini $CURRENT_DIR/boids.ini
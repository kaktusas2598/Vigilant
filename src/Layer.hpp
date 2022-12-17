#ifndef __LAYER__
#define __LAYER__

namespace Vigilant {

    class Layer {
        public:
            virtual void init() {};
            virtual void update() = 0;
            virtual void render() = 0;
        //protected:
            virtual ~Layer() {}
    };
}

#endif // __LAYER__

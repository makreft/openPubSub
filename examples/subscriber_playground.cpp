#include "../openPubSub/subscription.h"

int main()
{
    openPubSub::Subscriber sub;
    openPubSub::init(sub);
    sub.run();
}

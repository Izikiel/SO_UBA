class Proc
{
    public:
        Proc(): start(0), deadline(0), pid(-1) {};
        Proc(int new_start, int new_deadline, int new_pid):
            start(new_start), deadline(new_deadline), pid(new_pid) {};
        ~Proc() {};
        int priority() {return deadline - start;};
        bool operator >(Proc b)
        {
            return priority() > b.priority();
        }
        bool operator <(Proc b)
        {
            return priority() < b.priority();
        }
        bool operator ==(Proc b)
        {
            return (get_start() == b.get_start()) &&
                   (get_deadline() == b.get_deadline()) &&
                   (get_pid() == b.get_pid());
        }
        int get_start() {return start;}
        int get_deadline() {return deadline;}
        int get_pid() {return pid;}
        void tick() {start += 1;}
    private:
        int start, deadline, pid;
};
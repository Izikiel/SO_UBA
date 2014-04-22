class Proc
{
    public:
        Proc(): deadline(0), pid(-1) {};
        Proc(int new_deadline, int new_pid):
            deadline(new_deadline), pid(new_pid) {};
        ~Proc() {};
        bool operator >(Proc b) const
        {
            if (!get_deadline() and b.get_deadline()) {
                return false;

            } else if (!b.get_deadline() and get_deadline()) {
                return true;
            }

            return get_deadline() < b.get_deadline();
        }
        bool operator <(Proc b) const
        {
            if (!get_deadline() and b.get_deadline()) {
                return true;

            } else if (!b.get_deadline() and get_deadline()) {
                return false;
            }

            return get_deadline() > b.get_deadline();
        }
        bool operator ==(Proc b) const
        {
            return (get_deadline() == b.get_deadline()) &&
                   (get_pid() == b.get_pid());
        }
        int get_deadline() const {return deadline;}
        int get_pid() const {return pid;}
    private:
        int deadline;
        int pid;
};